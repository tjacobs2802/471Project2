// CPaintEffect.cpp
#include "CPaintEffect.h"
#include <cmath>

CPaintEffect::CPaintEffect() : m_brushSize(5), m_brushR(255), m_brushG(0), m_brushB(0) {}

void CPaintEffect::SetBrushSize(int size)
{
    m_brushSize = size;
}

void CPaintEffect::SetBrushColor(int r, int g, int b)
{
    m_brushR = r;
    m_brushG = g;
    m_brushB = b;
}

void CPaintEffect::Paint(CGrImage& image, int x, int y)
{
    for (int dy = -m_brushSize; dy <= m_brushSize; ++dy)
    {
        for (int dx = -m_brushSize; dx <= m_brushSize; ++dx)
        {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < image.GetWidth() && ny >= 0 && ny < image.GetHeight())
            {
                double distance = sqrt(dx * dx + dy * dy);
                if (distance <= m_brushSize)
                {
                    image.Set(nx, ny, m_brushR, m_brushG, m_brushB);
                }
            }
        }
    }
}


int CPaintEffect::GetBrushSize() const
{
    return m_brushSize;
}

int CPaintEffect::GetBrushR() const
{
    return m_brushR;
}

int CPaintEffect::GetBrushG() const
{
    return m_brushG;
}

int CPaintEffect::GetBrushB() const
{
    return m_brushB;
}