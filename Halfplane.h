#ifndef HALFPLANE_H
#define HALFPLANE_H
#include "Utility.h"

class HalfPlane
{
    private:
        Point p1;
        Point p2;
        //ax+by+c=0
        int a;
        int b;
        int c;
        int sign; //-1 (lower) < and 1 (upper) >

    public:
        HalfPlane():a(0),b(0),c(0),sign(0),p1(0,0),p2(0,0){}
        HalfPlane(int a, int b, int c, int s);
        HalfPlane(int a, int b, int c, int s, Point p1, Point p2);

        const Point getP1() const {return p1;}
        const Point getP2() const {return p2;}
        const int getA() const {return a;}
        const int getB() const {return b;}
        const int getC() const {return c;}
        const int getSign() const {return sign;}

        bool isBelong(const Point& p) const;
        vector<Point> getPoints(const Point bg, const Point hd);

        HalfPlane transformHalfplane(const transformation t) const;
        HalfPlane transformPoint(const transformation t) const;
        double distancePoint(const Point p);
};

bool isBelong(const vector<HalfPlane>& vecHP, const Point& p);

#endif // HALFPLANE_H

