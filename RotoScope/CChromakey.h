#pragma once
#include "graphics/GrImage.h"

class CChromakey {
public:
    CChromakey(double greenThreshold, int minGreenIntensity, double shadowToleranceFactor);

    // Perform chromakeying
    CGrImage Apply(const CGrImage& foreground, const CGrImage& background);

    // Generate an automatic garbage mask
    CGrImage GenerateGarbageMask(const CGrImage& frame);

private:
    double m_greenThreshold;
    int m_minGreenIntensity;
    double m_shadowToleranceFactor;
    CGrImage m_garbageMask;

    void ResizeGarbageMask(int width, int height);
};