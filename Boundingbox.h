#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "Utility.h"
#include "definition.h"

#define EPSILON 1e-6
//#define EPSILON 0.05

class BoundingBox
{
        PointD topLeft;
        PointD bottomLeft;
        PointD bottomRight;
        PointD topRight;
    public:
        BoundingBox();
        BoundingBox(PointD p1, PointD p2, PointD p3, PointD p4);
        BoundingBox(const DigitalStandardSegment4& s);

        const PointD getPointMin() const {return bottomLeft;}
        const PointD getPointMax() const {return topRight;}
        const PointD getTopLeft() const {return topLeft;}
        const PointD getBottomLeft() const {return bottomLeft;}
        const PointD getBottomRight() const {return bottomRight;}
        const PointD getTopRight() const {return topRight;}
        void getBoundingBox(PointD& p1, PointD& p2, PointD& p3, PointD& p4) const;
        BoundingBox getBoundingBox() const;
        Point getSize();

        void setTopLeft(const PointD p){topLeft=p;}
        void setBottomLeft(const PointD p){bottomLeft=p;}
        void setBottomRight(const PointD p){bottomRight=p;}
        void setTopRight(const PointD p){topRight=p;}
        void setBoundingBox(const PointD p1, const PointD p2, const PointD p3, const PointD p4);

        BoundingBox transformBoundingBox(const transformation t);
        BoundingBox transformBoundingBox(const transformation T, int a, int b, int mu);
        BoundingBox transformBoundingBox(const transformation T, int a, int b, int mu, int imgSize);
        void draw(Board2D& aBoard) const;
        bool isInside(const Point& p) const;
        vector<Point> getInsidePoints() const;
};

BoundingBox findBoundingBox(const vector<Point>& vec);
BoundingBox findBoundingBox(const vector<RealPoint>& vec);
BoundingBox findBoundingBox(const IplImage* img, CvScalar color);

#endif // BOUNDINGBOX_H
