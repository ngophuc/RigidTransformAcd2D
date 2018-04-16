#ifndef UTLITY_H
#define UTLITY_H
#include "DGtal/images/ImageSelector.h"
#include "DGtal/io/readers/PGMReader.h"
#include "DGtal/io/writers/PGMWriter.h"
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"

#include <stdio.h>
#include <vector>
#include <algorithm>

#include "cv.h"
#include "highgui.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

typedef vector<Point>::const_iterator ConstIterator;
typedef ImageSelector <Domain, unsigned char>::Type Image;
typedef double transformation[3]; // a transformation (a,b,theta) !

#define ESP_DOUBLE 1e-6
#define DELTA 0.001
#define OBJ 255

/********** Find element **********/
bool isExist(const vector<int>& vec, int v);
bool isExist(const vector<Point>& vec, Point p);
bool isExist(const vector<RealPoint>& vec, Point p);
int findElement(const vector<Point>& vec, Point p);
int findElement(const vector<RealPoint>& vec, Point p);
/********** Find element **********/

/********** Distance function **********/
double distancePoints(Point p1, Point p2);
double distanceLine(Point p, int a, int b, int c);
double distanceLine(Point p, Point s1, Point s2);
//If d<0 then the point lies on left side, and if d>0 then it lies on the right side, else it lies exactly on line.
double signedDistanceLine(Point p, int a, int b, int c);
double signedDistanceLine(Point p, Point s1, Point s2);
/********** Distance function **********/

/********* Angle function *********/
double relativeAngle(Point v1, Point v2); //This gives us just the relative angle between a and b, in [0,PI].
double relativeAngle(Point p1, Point p2, Point p3);
double signedAngle(Point v1, Point v2); //This gives us the relative angle between a and b with a sign, in [-PI,PI]
double signedAngle(Point p1, Point p2, Point p3);
double acuteAngle(Point p1, Point p2, Point p3); // acute angle between three points
double simpleAngle(Point p1, Point p2, Point p3); // angle between three points

void transformPythagoreAngle(const transformation t,const size_t imgSize, int& a, int& b, int& c);
/********* Angle function *********/

/******** Trasformation *********/
vector<Point> translateVector(const vector<Point>& vec, const Point& p);

RealPoint forwardTransformPoint(Point p, const transformation t);
RealPoint backwardTransformPoint(Point p, const transformation t);
RealPoint forwardTransformPoint(RealPoint p, const transformation t);
RealPoint backwardTransformPoint(RealPoint p, const transformation t);

RealPoint transformPoint(const RealPoint p, const transformation t);
RealPoint transformPoint(const RealPoint p, const transformation t, const size_t imgSize);
RealPoint transformPoint(const RealPoint p, const RealPoint center, const transformation t);
/******** Trasformation *********/

/******** Projection *******/
RealPoint projectPoint(const double aa, const double bb, const double cc, const RealPoint& aM);
RealPoint projectPoint(const double aa, const double bb, const Point& aM, const double aR);
/******** Projection *******/

/********** Conversion of transformation **********/
int pgcd(int a, int b);
double cross(const Point &p0, const Point &p1, const Point &p2);
double scalarProduct(const RealPoint& v1, const RealPoint& v2);
RealPoint vectorValue(const RealPoint& p1, const RealPoint& p2);
int findSmallestValue(double X, double Y);
bool convertDecimal2Fraction(double DecimalNum, int& Numerator, int& Denominator);
bool convertAngle2Pythagore(double angle, int& a, int& b, int& c, double e=1e-5);
/********** Conversion of transformation **********/

/******* Polygone ********/
bool isInPolygon(const vector<Point>& polygon, Point p);
bool isInPolygon(const vector<vector<Point> >& polygon, Point p);
bool isInPolygon(const vector<RealPoint>& polygon, Point p);
/******* Polygone ********/

/******* Bounding box ********/
void findBoundingBox(const vector<Point>& vec, Point& pMin, Point& pMax);
void findBoundingBox(const vector<vector<Point> >& vec, Point& pMin, Point& pMax);
void findBoundingBox(const vector<RealPoint>& vec, Point& pMin, Point& pMax);
/******* Bounding box ********/

RealPoint findCentroid(const vector<Point>& vec);
RealPoint findCentroid(const vector<RealPoint>& vec);
vector<Point> pointAlongEdge(Point p1, Point p2);
vector<Point> pointAlongEdge(RealPoint p1, RealPoint p2);

/****** Get slope *******/
double getSlope(double Nx,double Ny);
double getSlope(RealPoint N);
double getSlope(double Nx, double Ny, int sign);
/****** Get slope *******/

#endif // UTLITY_H
