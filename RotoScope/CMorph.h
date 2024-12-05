#pragma once
#include "graphics/GrImage.h"
#include <vector>

class CMorph
{
public:
    // Constructor
    CMorph();

    // Morph function
    CGrImage MorphImages(CGrImage& srcImage, CGrImage& destImage, double alpha);

private:
    std::vector<CPoint> m_destPoints = { {134, 77}, {159, 777}, {195,102}, {153,129}, {71,95}, {127,90}, {181,141} };  // Mario's features
    std::vector<CPoint> m_srcPoints = { {34, 15}, {39, 15}, {36, 18}, { 36,20 }, {31,16}, {43,16}, {36, 24} };  // Left eye, right eye, nose, mouth, left ear, right ear, chin

    CPoint InterpolatePoint(const CPoint& p1, const CPoint& p2, double alpha);
};