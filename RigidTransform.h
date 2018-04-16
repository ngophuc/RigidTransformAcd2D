#ifndef RIGIDTRANSFORM_H
#define RIGIDTRANSFORM_H

#include "Utility.h"

/*** Transform points ***/
Image transfomPoints(string filename, transformation T, int obj=0);
/*** Transform polygone ***/
vector<vector<Point> > transformPolygon(const vector<vector<Point> >& polyLine, vector<vector<RealPoint> >& tPolyLine, transformation t);

vector<RealPoint> transformPolygon(const vector<Point>& polyLine, transformation t);
vector<Point> transformPolygonRound(const vector<Point>& polyLine, transformation t);

vector<RealPoint> transformPolygon(const vector<Point>& polyLine, transformation t, RealPoint c);
vector<Point> transformPolygonRound(const vector<Point>& polyLine, transformation t, RealPoint c);

#endif // RIGIDTRANSFORM_H
