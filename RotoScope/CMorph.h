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
    std::vector<CPoint> m_destPoints = { {99,186},{80,151},{114,149},{96,119},{83,137},{90,136},{104,135},{111,136},{98,145},{95,148},{101,148},{89,161},{108,159},{97,152},{98,173} }; // Trevor
    std::vector<CPoint> m_srcPoints = { {288,210},{264,170},{317,171},{290,122},{264,170},{278,149},{301,149},{312,148},{287,170},{283,175},{295,174},{272,191},{304,195},{288,182},{288,188} };  // Julia
    CPoint InterpolatePoint(const CPoint& p1, const CPoint& p2, double alpha);
    CGrImage ResizeImage(CGrImage& image, int newWidth, int newHeight);
};

/*
1. Chin
2. Left cheek
3. Right cheek
4. Forehead
5. Left eye(outer corner)
6. Left eye(inner corner)
7. Right eye(inner corner)
8. Right eye(outer corner)
9. Nose tip
10. Left nostril
11. Right nostril
12. Mouth(left corner)
13. Mouth(right corner)
14. Upper lip(center)
15. Lower lip(center)
*/