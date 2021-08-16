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

            int preObjID = (int)m_preFrameInfo.m_id(preScreenPosition.x, preScreenPosition.y);

            // in screen and preObjID == objID
            if (preScreenPosition.x >= 0 && preScreenPosition.x < width &&
                preScreenPosition.y >= 0 && preScreenPosition.y < height &&
                preObjID == objID) {
                m_valid(x, y) = true;
                m_misc(x, y) = m_preFrameInfo.m_beauty(x, y);
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
            Float3 color = m_accColor(x, y);
            // TODO: Exponential moving average
            float alpha = 1.0f;
            m_misc(x, y) = Lerp(color, curFilteredColor(x, y), alpha);
        }
    }
    std::swap(m_misc, m_accColor);
}

Buffer2D<Float3> Denoiser::Filter(const FrameInfo &frameInfo) {
    int height = frameInfo.m_beauty.m_height;
    int width = frameInfo.m_beauty.m_width;
    Buffer2D<Float3> filteredImage = CreateBuffer2D<Float3>(width, height);
    int kernelRadius = 16;
#pragma omp parallel for
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // TODO: Joint bilateral filter
            //filteredImage(x, y) = frameInfo.m_beauty(x, y);

            float sumWeight = 0;
            Float3 sumColor(0);
            for (int i = std::max(0, x - kernelRadius / 2); i < std::min(x + kernelRadius / 2, width); i++) {
                for (int j = std::max(0, y - kernelRadius / 2); j < std::min(y + kernelRadius / 2, height); j++) {
                    float w_g =
                        (Sqr(i - x) + Sqr(j - y)) /
                        (2.0f * Sqr(m_sigmaCoord));
                    float w_color =
                        SqrLength(frameInfo.m_beauty(i, j) - frameInfo.m_beauty(x, y)) /
                        (2.0f * Sqr(m_sigmaColor));
                    float dotnormal = Dot(frameInfo.m_normal(i, j), frameInfo.m_normal(x, y));
                    if (dotnormal == 0.0f) continue; // one of normal is [0, 0, 0] or two normals are vertical
                    float w_normal =
                        Sqr(SafeAcos(dotnormal)) /
                        (2.0f * Sqr(m_sigmaNormal));
                    float sqrposdis = SqrDistance(frameInfo.m_position(i, j), frameInfo.m_position(x, y));
                    if (sqrposdis == 0.0f) continue; // two positions are parallel
                    float w_plane =
                        Sqr(Dot(frameInfo.m_normal(x, y), frameInfo.m_position(i, j) - frameInfo.m_position(x, y))) / sqrposdis /
                        (2.0f * Sqr(m_sigmaPlane));

                    float weight = exp((w_g + w_color + w_normal + w_plane) * -1);
                    sumWeight += weight;
                    sumColor += frameInfo.m_beauty(i, j) * weight;
                }
            }
            if (sumWeight > 0.0f) sumColor /= sumWeight;
            filteredImage(x, y) = sumColor;
        }
    }
    return filteredImage;
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
