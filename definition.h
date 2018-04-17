#ifndef DEFINITION_H
#define DEFINITION_H

#include <numeric>
#include <algorithm>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <exception>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "DGtal/geometry/curves/ArithmeticalDSS.h"
#include "DGtal/base/BasicTypes.h"
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/helpers/StdDefs.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

#define FILENAMESIZE 200

#define ERROR 1e-5

namespace mySpace {
    typedef PointVector<2, double> PointD;
    typedef PointVector<2,int> Point;
    typedef PointVector<3, double> Point3DD;
    typedef PointVector<3,int> Point3D;
    typedef std::vector<Point> Range;
    typedef std::vector<Point>::const_iterator ConstIterator;
    typedef std::vector<PointD>::const_iterator ConstIteratorD;
    typedef std::vector<Point3D>::const_iterator ConstIterator3D;
    typedef std::vector<Point3DD>::const_iterator ConstIterator3DD;

    typedef ArithmeticalDSSComputer<ConstIterator,int,4> DigitalStandardSegment4;
}

using namespace mySpace;

#endif // DEFINITION_H
