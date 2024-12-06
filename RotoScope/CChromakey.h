#pragma once
#include "graphics/GrImage.h"
#include <string>

class CChromakey {
public:
    // Constructor with chroma key color (green or blue)
    CChromakey(double chromaThreshold, int minChromaIntensity, double shadowToleranceFactor, const std::string& chromaKeyColor);

    // Perform chromakeying
    CGrImage Apply(const CGrImage& foreground, const CGrImage& background);

    // Generate an automatic garbage mask
    CGrImage GenerateGarbageMask(const CGrImage& frame);

private:
    double m_chromaThreshold;              // Threshold for chroma dominance
    int m_minChromaIntensity;             // Minimum intensity for chroma key
    double m_shadowToleranceFactor;       // Tolerance factor for shadows
    std::string m_chromaKeyColor;         // Chroma key color ("green" or "blue")
    CGrImage m_garbageMask;               // Garbage mask for chroma keying

    void ResizeGarbageMask(int width, int height); // Resize garbage mask to match image dimensions
};
