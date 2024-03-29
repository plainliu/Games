#pragma once

#define NOMINMAX
#include <string>

#include "filesystem/path.h"

#include "util/image.h"
#include "util/mathutil.h"

struct FrameInfo {
  public:
    Buffer2D<Float3> m_beauty;
    Buffer2D<float> m_depth;
    Buffer2D<Float3> m_normal;
    Buffer2D<Float3> m_position;
    Buffer2D<float> m_id;
    std::vector<Matrix4x4> m_matrix;
};

class Denoiser {
  public:
    Denoiser();

    void Init(const FrameInfo &frameInfo, const Buffer2D<Float3> &filteredColor);
    void Maintain(const FrameInfo &frameInfo);

    void Reprojection(const FrameInfo &frameInfo);
    void TemporalAccumulation(const Buffer2D<Float3> &curFilteredColor);
    Buffer2D<Float3> Filter(const FrameInfo &frameInfo);
    Float3 NormalLevelFilter(const FrameInfo &frameInfo, const int &width,
                             const int &height, const int &x, const int &y,
                             const int &kernelRadius);
    Float3 ATrousWaveletLevelFilter(const FrameInfo &frameInfo,
                                    const Buffer2D<Float3> &curFilteredColor,
                                    const int &width, const int &height, const int &x,
                                    const int &y, const int &level);
    Float3 LevelFilter(const FrameInfo &frameInfo,
                       const Buffer2D<Float3> &curFilteredColor, const int &width,
                       const int &height, const int &x, const int &y,
                       const int &level = 0,
                       const int &kernelRadius = 2);

    Buffer2D<Float3> ProcessFrame(const FrameInfo &frameInfo);

  public:
    FrameInfo m_preFrameInfo;
    Buffer2D<Float3> m_accColor;
    Buffer2D<Float3> m_misc;
    Buffer2D<bool> m_valid;
    bool m_useTemportal;

    float m_alpha = 0.2f;
    float m_sigmaPlane = 0.1f;
    float m_sigmaNormal = 0.1f;
    float m_sigmaCoord = 32.0f;
    float m_colorBoxK = 100.0f;

    //���ز���
    //#define _MYPC_MSC
    //#define _INPUT_BOX

    #ifdef _MYPC_MSC

    #ifdef _INPUT_BOX
    float m_sigmaColor = 0.6f;
    #else
    float m_sigmaColor = 10.6f;
    #endif

    #else
    float m_sigmaColor = 0.6f;
    #endif
};