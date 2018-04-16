#ifndef CONNECTEDCOMPONENT_H
#define CONNECTEDCOMPONENT_H

#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include <DGtal/kernel/domains/HyperRectDomain.h>
#include <DGtal/kernel/SpaceND.h>
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/io/colormaps/GrayscaleColorMap.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
#include "DGtal/images/ConstImageAdapter.h"
#include "DGtal/io/readers/PGMReader.h"
#include "DGtal/io/writers/PGMWriter.h"

#include "DGtal/base/IteratorCirculatorTraits.h"
#include "DGtal/geometry/tools/Hull2DHelpers.h"
#include "DGtal/geometry/tools/PolarPointComparatorBy2x2DetComputer.h"
#include "DGtal/geometry/tools/determinant/AvnaimEtAl2x2DetSignComputer.h"
#include "DGtal/geometry/tools/determinant/InHalfPlaneBySimple3x3Matrix.h"
#include "DGtal/shapes/ShapeFactory.h"
#include "DGtal/shapes/Shapes.h"
#include "DGtal/topology/DigitalSetBoundary.h"
#include "DGtal/topology/DigitalSurface.h"
#include "DGtal/graph/DepthFirstVisitor.h"

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i;

typedef struct
{
        int id;
        vector<Point> points;
        Color color;
}Region;


typedef ImageContainerBySTLVector<Domain, unsigned char> Image;
typedef ConstImageAdapter<Image, Domain, functors::Identity, bool, DGtal::functors::Thresholder<Image::Value> > ConstImageAdapterForThresholderImage;
typedef GrayscaleColorMap<unsigned char> Gray;

int getConnectedComponent(string input, string output, int threshold, int typeConnectedObject, int typeConnectedBackground);

#endif // CONNECTEDCOMPONENT_H
