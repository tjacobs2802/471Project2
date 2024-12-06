#include "CChromakey.h"
#include <stdexcept>
#include <cmath>

CChromakey::CChromakey(double greenThreshold, int minGreenIntensity, double shadowToleranceFactor) {
    m_greenThreshold = greenThreshold;
    m_minGreenIntensity = minGreenIntensity;
    m_shadowToleranceFactor = shadowToleranceFactor;
    m_garbageMask.LoadFile(L"garbagemask.png");
}

void CChromakey::ResizeGarbageMask(int width, int height) {
    if (m_garbageMask.GetWidth() != width || m_garbageMask.GetHeight() != height) {
        CGrImage resizedImage;
        resizedImage.SetSize(width, height); // Create a new image with the desired dimensions

        int originalWidth = m_garbageMask.GetWidth();
        int originalHeight = m_garbageMask.GetHeight();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Map the new pixel location to the original image's coordinates
                int srcX = x * originalWidth / width;
                int srcY = y * originalHeight / height;

                // Copy the nearest pixel's RGB values
                const unsigned char* srcPixel = m_garbageMask[srcY] + srcX * 3;
                unsigned char* destPixel = resizedImage[y] + x * 3;

                destPixel[0] = srcPixel[0]; // R
                destPixel[1] = srcPixel[1]; // G
                destPixel[2] = srcPixel[2]; // B
            }
        }

        // Update m_garbageMask with the resized image
        m_garbageMask = std::move(resizedImage);
    }
}

CGrImage CChromakey::Apply(const CGrImage& foreground, const CGrImage& background) {
    if (foreground.GetWidth() != background.GetWidth() || foreground.GetHeight() != background.GetHeight()) {
        throw std::runtime_error("Foreground and background must have the same dimensions.");
    }

    int width = foreground.GetWidth();
    int height = foreground.GetHeight();

    // Resize the garbage mask to match the size of the foreground/background
    ResizeGarbageMask(width, height);

    // Create the output image
    CGrImage output;
    output.SetSameSize(foreground);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = x * 3;

            // Foreground pixel components
            const unsigned char* fgRow = foreground[y];
            double fgR = static_cast<double>(fgRow[idx]);
            double fgG = static_cast<double>(fgRow[idx + 1]);
            double fgB = static_cast<double>(fgRow[idx + 2]);

            // Background pixel components
            const unsigned char* bgRow = background[y];
            double bgR = static_cast<double>(bgRow[idx]);
            double bgG = static_cast<double>(bgRow[idx + 1]);
            double bgB = static_cast<double>(bgRow[idx + 2]);

            // Garbage mask pixel
            const unsigned char* maskRow = m_garbageMask[y];
            double maskValue = static_cast<double>(maskRow[idx]) / 255.0;

            // Compute green dominance
            double greenDominance = fgG / (fgR + fgG + fgB + 0.001);

            // Compute alpha
            double alpha = 1.0;
            if (fgG > m_minGreenIntensity && fgG > fgR * m_shadowToleranceFactor &&
                fgG > fgB * m_shadowToleranceFactor && greenDominance > m_greenThreshold) {
                alpha = 0.0;
            }

            // Modify alpha based on garbage mask
            if (maskValue == 0.0) {
                // Garbage mask is black: use background pixel directly
                alpha = 0.0;
            }
            else if (maskValue == 1.0) {
                // Garbage mask is white: fully apply alpha blending (keep calculated alpha)
            }
            else {
                // Garbage mask is gray: interpolate between alpha blending and background
                alpha = alpha * maskValue;
            }

            // Compute the output pixel
            double outR = alpha * fgR + (1.0 - alpha) * bgR;
            double outG = alpha * fgG + (1.0 - alpha) * bgG;
            double outB = alpha * fgB + (1.0 - alpha) * bgB;

            // Write to output
            unsigned char* outRow = output[y];
            outRow[idx] = static_cast<unsigned char>(outR);
            outRow[idx + 1] = static_cast<unsigned char>(outG);
            outRow[idx + 2] = static_cast<unsigned char>(outB);
        }
    }

    return output;
}
// Aidan's project
/*
void CRotoScopeDoc::Chromakey(CGrImage& foreground, CGrImage& background, CGrImage& output, CGrImage& garbageMask)
{
    // Define the target key color for blue screen
    const double key_r = 0.0; // Red component of blue screen color
    const double key_g = 0.0; // Green component of blue screen color
    const double key_b = 255.0; // Blue component of blue screen color

    // Ensure the output image has the same dimensions as the foreground
    output.SetSameSize(foreground);

    // Iterate over every pixel in the foreground image
    for (int y = 0; y < foreground.GetHeight(); ++y)
    {
        for (int x = 0; x < foreground.GetWidth(); ++x)
        {
            // Foreground color
            double r = foreground[y][x * 3];
            double g = foreground[y][x * 3 + 1];
            double b = foreground[y][x * 3 + 2];

            // Background color
            double bg_r = background[y][x * 3];
            double bg_g = background[y][x * 3 + 1];
            double bg_b = background[y][x * 3 + 2];

            // Calculate alpha value using the Vlahos equation
            double alpha = 1.0 - min(1.0, (b - key_b) / 255.0);
            alpha = max(0.0, min(alpha, 1.0)); // Ensure alpha is within [0, 1]

            // Apply the garbage mask to adjust the alpha value
            double mask_value = garbageMask[y][x * 3] / 255.0; // Normalize the garbage mask pixel to [0, 1]
            alpha *= (1.0 - mask_value); // Reduce alpha based on the garbage mask

            // Blend the pixels based on the alpha value
            double out_r = alpha * r + (1.0 - alpha) * bg_r;
            double out_g = alpha * g + (1.0 - alpha) * bg_g;
            double out_b = alpha * b + (1.0 - alpha) * bg_b;

            // Write the blended color to the output image
            output.Set(x, y, static_cast<int>(out_r), static_cast<int>(out_g), static_cast<int>(out_b));
        }
    }
}
*/