#include "CMorph.h"
#include <cmath>
#include <algorithm> // Ensure the standard library functions are available

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

// Constructor
CMorph::CMorph() {}

// Interpolate between two points
CPoint CMorph::InterpolatePoint(const CPoint& p1, const CPoint& p2, double alpha)
{
    int x = static_cast<int>(p1.x * (1.0 - alpha) + p2.x * alpha);
    int y = static_cast<int>(p1.y * (1.0 - alpha) + p2.y * alpha);
    return CPoint(x, y);
}

// Morph function
CGrImage CMorph::MorphImages(CGrImage& srcImage, CGrImage& destImage, double alpha)
{
    // Initialize the output image
    CGrImage outputImage;
    outputImage.SetSameSize(srcImage);

    // Compute interpolated control points
    std::vector<CPoint> interpolatedPoints;
    for (size_t i = 0; i < m_srcPoints.size(); ++i) {
        interpolatedPoints.push_back(InterpolatePoint(m_srcPoints[i], m_destPoints[i], alpha));
    }

    // Perform basic pixel mapping
    for (int y = 0; y < srcImage.GetHeight(); ++y) {
        for (int x = 0; x < srcImage.GetWidth(); ++x) {
            double dx = 0.0, dy = 0.0, weightSum = 0.0;

            for (size_t i = 0; i < m_srcPoints.size(); ++i) {
                double distance = std::sqrt(std::pow(m_srcPoints[i].x - x, 2) + std::pow(m_srcPoints[i].y - y, 2)) + 1e-5;
                double weight = 1.0 / distance;

                dx += weight * (interpolatedPoints[i].x - m_srcPoints[i].x);
                dy += weight * (interpolatedPoints[i].y - m_srcPoints[i].y);
                weightSum += weight;
            }

            int mappedX = static_cast<int>(x + dx / weightSum);
            int mappedY = static_cast<int>(y + dy / weightSum);

            // Ensure mapped points are within bounds
            mappedX = std::max(0, std::min(srcImage.GetWidth() - 1, mappedX));
            mappedY = std::max(0, std::min(srcImage.GetHeight() - 1, mappedY));

            // Blend pixel values
            for (int c = 0; c < 3; ++c) {
                int srcValue = srcImage[y][x * 3 + c];
                int destValue = destImage[mappedY][mappedX * 3 + c];
                outputImage[y][x * 3 + c] = static_cast<int>(srcValue * (1.0 - alpha) + destValue * alpha);
            }
        }
    }

    return outputImage;
}