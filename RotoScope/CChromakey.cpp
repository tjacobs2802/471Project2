#include "CChromakey.h"
#include <stdexcept>
#include <cmath>

CChromakey::CChromakey(double chromaThreshold, int minChromaIntensity, double shadowToleranceFactor, const std::string& chromaKeyColor)
    : m_chromaThreshold(chromaThreshold),
    m_minChromaIntensity(minChromaIntensity),
    m_shadowToleranceFactor(shadowToleranceFactor),
    m_chromaKeyColor(chromaKeyColor)
{
    if (m_chromaKeyColor != "green" && m_chromaKeyColor != "blue")
    {
        throw std::invalid_argument("Chroma key color must be 'green' or 'blue'.");
    }

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

CGrImage CChromakey::Apply(const CGrImage& foreground, const CGrImage& background)
{
    int fgWidth = foreground.GetWidth();
    int fgHeight = foreground.GetHeight();
    int bgWidth = background.GetWidth();
    int bgHeight = background.GetHeight();

    // Ensure the background matches the foreground dimensions
    CGrImage adjustedBackground;
    if (fgWidth != bgWidth || fgHeight != bgHeight)
    {
        adjustedBackground.SetSize(fgWidth, fgHeight);

        for (int y = 0; y < fgHeight; y++)
        {
            for (int x = 0; x < fgWidth; x++)
            {
                // Map the new pixel location to the background's coordinates
                int srcX = x * bgWidth / fgWidth;
                int srcY = y * bgHeight / fgHeight;

                // Copy the nearest pixel's RGB values from the background
                const unsigned char* srcPixel = background[srcY] + srcX * 3;
                unsigned char* destPixel = adjustedBackground[y] + x * 3;

                destPixel[0] = srcPixel[0]; // R
                destPixel[1] = srcPixel[1]; // G
                destPixel[2] = srcPixel[2]; // B
            }
        }
    }
    else
    {
        adjustedBackground = background; // Use the original background if sizes match
    }

    // Resize the garbage mask to match the foreground/background
    ResizeGarbageMask(fgWidth, fgHeight);

    // Create the output image
    CGrImage output;
    output.SetSameSize(foreground);

    for (int y = 0; y < fgHeight; y++)
    {
        for (int x = 0; x < fgWidth; x++)
        {
            int idx = x * 3;

            // Foreground pixel components
            const unsigned char* fgRow = foreground[y];
            double fgR = static_cast<double>(fgRow[idx]);
            double fgG = static_cast<double>(fgRow[idx + 1]);
            double fgB = static_cast<double>(fgRow[idx + 2]);

            // Adjusted background pixel components
            const unsigned char* bgRow = adjustedBackground[y];
            double bgR = static_cast<double>(bgRow[idx]);
            double bgG = static_cast<double>(bgRow[idx + 1]);
            double bgB = static_cast<double>(bgRow[idx + 2]);

            // Garbage mask pixel
            const unsigned char* maskRow = m_garbageMask[y];
            double maskValue = static_cast<double>(maskRow[idx]) / 255.0;

            // Compute chroma dominance
            double chromaDominance;
            if (m_chromaKeyColor == "green")
            {
                chromaDominance = fgG / (fgR + fgG + fgB + 0.001);
            }
            else if (m_chromaKeyColor == "blue")
            {
                chromaDominance = fgB / (fgR + fgG + fgB + 0.001);
            }


            // Compute alpha
            double alpha = 1.0;
            if ((m_chromaKeyColor == "green" && fgG > m_minChromaIntensity && fgG > fgR * m_shadowToleranceFactor && fgG > fgB * m_shadowToleranceFactor && chromaDominance > m_chromaThreshold) ||
                (m_chromaKeyColor == "blue" && fgB > m_minChromaIntensity && fgB > fgR * m_shadowToleranceFactor && fgB > fgG * m_shadowToleranceFactor && chromaDominance > m_chromaThreshold))
            {
                alpha = 0.0;
            }

            // Modify alpha based on garbage mask
            if (maskValue == 0.0)
            {
                alpha = 0.0; // Garbage mask is black: use background pixel directly
            }
            else if (maskValue == 1.0)
            {
                // Garbage mask is white: fully apply alpha blending (keep calculated alpha)
            }
            else
            {
                alpha = alpha * maskValue; // Garbage mask is gray: interpolate between alpha blending and background
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

