#include "CMorph.h"
#include <cmath>
#include <algorithm>

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

// Resize function
CGrImage CMorph::ResizeImage(CGrImage& image, int newWidth, int newHeight)
{
    CGrImage resizedImage;
    resizedImage.SetSize(newWidth, newHeight);

    double scaleX = static_cast<double>(image.GetWidth()) / newWidth;
    double scaleY = static_cast<double>(image.GetHeight()) / newHeight;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            // Map the target pixel to the source image
            double srcX = x * scaleX;
            double srcY = y * scaleY;

            // Calculate integer pixel indices
            int x1 = static_cast<int>(std::floor(srcX));
            int y1 = static_cast<int>(std::floor(srcY));
            int x2 = std::min(x1 + 1, image.GetWidth() - 1);
            int y2 = std::min(y1 + 1, image.GetHeight() - 1);

            // Ensure indices are within bounds
            x1 = (x1 < 0) ? 0 : ((x1 >= image.GetWidth()) ? (image.GetWidth() - 1) : x1);
            y1 = (y1 < 0) ? 0 : ((y1 >= image.GetHeight()) ? (image.GetHeight() - 1) : y1);

            // Fractional parts for interpolation
            double dx = srcX - x1;
            double dy = srcY - y1;

            // Get the four neighboring pixels
            unsigned char* p11 = &image[y1][x1 * 3]; // Top-left
            unsigned char* p21 = &image[y1][x2 * 3]; // Top-right
            unsigned char* p12 = &image[y2][x1 * 3]; // Bottom-left
            unsigned char* p22 = &image[y2][x2 * 3]; // Bottom-right

            // Interpolate each color channel
            for (int c = 0; c < 3; ++c) { // RGB channels
                double val =
                    (1 - dx) * (1 - dy) * p11[c] +
                    dx * (1 - dy) * p21[c] +
                    (1 - dx) * dy * p12[c] +
                    dx * dy * p22[c];

                resizedImage[y][x * 3 + c] = static_cast<unsigned char>(std::round(val));
            }
        }
    }

    return resizedImage;
}

// Morph function
CGrImage CMorph::MorphImages(CGrImage& srcImage, CGrImage& destImage, double alpha) {
    // Resize both images to the same size
    int commonWidth = std::max(srcImage.GetWidth(), destImage.GetWidth());
    int commonHeight = std::max(srcImage.GetHeight(), destImage.GetHeight());

    CGrImage resizedSrcImage = ResizeImage(srcImage, commonWidth, commonHeight);
    CGrImage resizedDestImage = ResizeImage(destImage, commonWidth, commonHeight);

    int width = commonWidth;
    int height = commonHeight;

    // Create the resulting image
    CGrImage resultImage;
    resultImage.SetSize(width, height);

    // Morph each pixel
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Access each pixel using direct indexing
            unsigned char srcR = resizedSrcImage[y][x * 3];
            unsigned char srcG = resizedSrcImage[y][x * 3 + 1];
            unsigned char srcB = resizedSrcImage[y][x * 3 + 2];

            unsigned char destR = resizedDestImage[y][x * 3];
            unsigned char destG = resizedDestImage[y][x * 3 + 1];
            unsigned char destB = resizedDestImage[y][x * 3 + 2];

            // Blend colors based on alpha
            unsigned char resultR = static_cast<unsigned char>((1 - alpha) * srcR + alpha * destR);
            unsigned char resultG = static_cast<unsigned char>((1 - alpha) * srcG + alpha * destG);
            unsigned char resultB = static_cast<unsigned char>((1 - alpha) * srcB + alpha * destB);

            // Set the resulting pixel
            resultImage[y][x * 3] = resultR;
            resultImage[y][x * 3 + 1] = resultG;
            resultImage[y][x * 3 + 2] = resultB;
        }
    }

    return resultImage;
}