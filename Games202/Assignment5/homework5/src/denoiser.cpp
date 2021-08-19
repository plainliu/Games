#include "denoiser.h"

Denoiser::Denoiser() : m_useTemportal(false) {}

void Denoiser::Reprojection(const FrameInfo &frameInfo) {
    int height = m_accColor.m_height;
    int width = m_accColor.m_width;
    Matrix4x4 preWorldToScreen =
        m_preFrameInfo.m_matrix[m_preFrameInfo.m_matrix.size() - 1];
    Matrix4x4 preWorldToCamera =
        m_preFrameInfo.m_matrix[m_preFrameInfo.m_matrix.size() - 2];
#pragma omp parallel for
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // TODO: Reproject

            int objID = (int)frameInfo.m_id(x, y);
            if (objID == -1)
                continue;

            Matrix4x4 preMvp = preWorldToScreen * m_preFrameInfo.m_matrix[objID];
            Float3 preScreenPosition = (preMvp * Inverse(frameInfo.m_matrix[objID]))
                (frameInfo.m_position(x, y), Float3::Point);

            // in screen and preObjID == objID
            if (preScreenPosition.x >= 0 && preScreenPosition.x < width &&
                preScreenPosition.y >= 0 && preScreenPosition.y < height &&
                (int)m_preFrameInfo.m_id(preScreenPosition.x, preScreenPosition.y) == objID) {
                m_valid(x, y) = true;
                m_misc(x, y) = m_accColor(preScreenPosition.x, preScreenPosition.y);
            } else {
                m_valid(x, y) = false;
                m_misc(x, y) = Float3(0.f);
            }
        }
    }
    std::swap(m_misc, m_accColor);
}

void Denoiser::TemporalAccumulation(const Buffer2D<Float3> &curFilteredColor) {
    int height = m_accColor.m_height;
    int width = m_accColor.m_width;
    int kernelRadius = 3;
#pragma omp parallel for
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // TODO: Temporal clamp
            int count = 0;
            Float3 sumColor(0);
            Float3 sumSqrColor(0);
            for (int i = std::max(0, x - kernelRadius); i <= std::min(x + kernelRadius, width - 1); i++) {
                for (int j = std::max(0, y - kernelRadius); j <= std::min(y + kernelRadius, height - 1); j++) {
                    if (m_valid(i, j) == false)
                        continue;

                    sumColor += curFilteredColor(i, j);
                    sumSqrColor += Sqr(curFilteredColor(i, j));
                    count ++;
                }
            }
            Float3 color = m_accColor(x, y);
            if (count > 0) {
                sumColor /= count;
                sumSqrColor /= count;
                Float3 sigma = sumSqrColor - Sqr(sumColor);
                Clamp(color, sumColor - sigma * m_colorBoxK, sumColor + sigma * m_colorBoxK);
            }
            // TODO: Exponential moving average
            float alpha = m_valid(x, y) ? m_alpha : 1.0f;
            m_misc(x, y) = Lerp(color, curFilteredColor(x, y), alpha);
        }
    }
    std::swap(m_misc, m_accColor);
}

Buffer2D<Float3> Denoiser::Filter(const FrameInfo &frameInfo) {
    int height = frameInfo.m_beauty.m_height;
    int width = frameInfo.m_beauty.m_width;
    Buffer2D<Float3> filteredImage = CreateBuffer2D<Float3>(width, height);

//    // Joint bilateral filter
//    int kernelRadius = 16;
//#pragma omp parallel for
//    for (int y = 0; y < height; y++) {
//        for (int x = 0; x < width; x++) {
//            // TODO: Joint bilateral filter
//            // filteredImage(x, y) = frameInfo.m_beauty(x, y);
//
//            filteredImage(x, y) = NormalLevelFilter(frameInfo, height, width, x, y, kernelRadius);
//        }
//    }

    // A Trous Wavelet
    int waveletLevel = 3;
    Buffer2D<Float3> curFilteredColor = CreateBuffer2D<Float3>(width, height);
    filteredImage.Copy(frameInfo.m_beauty);
    for (int level = 0; level < waveletLevel; level++) {
#pragma omp parallel for
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                curFilteredColor(x, y) = ATrousWaveletLevelFilter(frameInfo, filteredImage, height, width, x, y, level);
            }
        }
        std::swap(curFilteredColor, filteredImage);
    }

    return filteredImage;
}

Float3 Denoiser::NormalLevelFilter(const FrameInfo &frameInfo, const int &width,
                                   const int &height, const int &x, const int &y,
                                   const int &kernelRadius) {
    return LevelFilter(frameInfo, frameInfo.m_beauty, width, height, x, y, 0, kernelRadius);
}

Float3 Denoiser::ATrousWaveletLevelFilter(const FrameInfo &frameInfo,
                                          const Buffer2D<Float3> &curFilteredColor,
                                          const int &width, const int &height,
                                          const int &x, const int &y, const int &level) {
    return LevelFilter(frameInfo, curFilteredColor, width, height, x, y, level, 2);
}

Float3 Denoiser::LevelFilter(const FrameInfo &frameInfo,
                             const Buffer2D<Float3> &curFilteredColor, const int &width,
                             const int &height, const int& x, const int& y,
                             const int& level, const int& kernelRadius) {
    int step = pow(2, level);

    float sumWeight = 0;
    Float3 sumColor(0);
    for (int i = -kernelRadius; i <= kernelRadius; i++) {
        for (int j = -kernelRadius; j <= kernelRadius; j++) {
            int x_ = x + i * step;
            int y_ = y + j * step;
            if (!(x_ >= 0 && y_ >= 0 && x_ < width && y_ < height))
                continue;

            float w_gauss = (Sqr(x_ - x) + Sqr(y_ - y)) / (2.0f * Sqr(m_sigmaCoord));

            float w_color = SqrLength(curFilteredColor(x_, y_) - curFilteredColor(x, y)) / (2.0f * Sqr(m_sigmaColor));

            float dotnormal = Dot(frameInfo.m_normal(x_, y_), frameInfo.m_normal(x, y));
            // one of normal is [0, 0, 0] or two normals are vertical
            if (dotnormal == 0.0f)
                continue;
            float w_normal = Sqr(SafeAcos(dotnormal)) / (2.0f * Sqr(m_sigmaNormal));

            float sqrposdis = SqrDistance(frameInfo.m_position(x_, y_), frameInfo.m_position(x, y));
            if (sqrposdis == 0.0f)
                continue;
            float w_plane = Sqr(Dot(frameInfo.m_normal(x, y), frameInfo.m_position(x_, y_) - frameInfo.m_position(x, y)))
                / sqrposdis / (2.0f * Sqr(m_sigmaPlane));

            float weight = exp((w_gauss + w_color + w_normal + w_plane) * -1);
            sumWeight += weight;
            sumColor += curFilteredColor(x_, y_) * weight;
        }
    }
    if (sumWeight > 0.0f)
        sumColor /= sumWeight;

    return sumColor;
}

void Denoiser::Init(const FrameInfo &frameInfo, const Buffer2D<Float3> &filteredColor) {
    m_accColor.Copy(filteredColor);
    int height = m_accColor.m_height;
    int width = m_accColor.m_width;
    m_misc = CreateBuffer2D<Float3>(width, height);
    m_valid = CreateBuffer2D<bool>(width, height);
}

void Denoiser::Maintain(const FrameInfo &frameInfo) { m_preFrameInfo = frameInfo; }

Buffer2D<Float3> Denoiser::ProcessFrame(const FrameInfo &frameInfo) {
    // Filter current frame
    Buffer2D<Float3> filteredColor;
    filteredColor = Filter(frameInfo);

    // Reproject previous frame color to current
    if (m_useTemportal) {
        Reprojection(frameInfo);
        TemporalAccumulation(filteredColor);
    } else {
        Init(frameInfo, filteredColor);
    }

    // Maintain
    Maintain(frameInfo);
    if (!m_useTemportal) {
        m_useTemportal = true;
    }
    return m_accColor;
}
