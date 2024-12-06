// CPaintEffect.h
#pragma once
#include "graphics/GrImage.h"

class CPaintEffect
{
public:
    CPaintEffect();
    void SetBrushSize(int size);
    void SetBrushColor(int r, int g, int b);
    void Paint(CGrImage& image, int x, int y);

    // Getters
    int GetBrushSize() const;
    int GetBrushR() const;
    int GetBrushG() const;
    int GetBrushB() const;

private:
    int m_brushSize;
    int m_brushR, m_brushG, m_brushB;
};