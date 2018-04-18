#include "Utility.h"
#include "ConnectedComponent.h"
//#include "RigidTransform.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
namespace po=boost::program_options;

/*** Connected component extraction ***/
int** readConnectedComponent(string filename, int& width, int& height);
vector<int> getLabelVector(int** tabLabels, int width, int height);
vector<vector<Point> > getLabelPoints(int** tabLabels, int width, int height);
/*** Contour extraction ***/
vector<vector<Point> > getBoundary4C(int** tabLabels, int width, int height, vector<vector<Point> >& vecPts);
vector<vector<Point> > getBoundary8C(int** tabLabels, int width, int height, vector<vector<Point> >& vecPts);
/*** Convex Hull ***/
vector<Point> computeConvexHull(const vector<Point>& vecPoint);
/*** Polygon extraction ***/
vector<Point> getPolygon(int** tabLabels, vector<Point>& vecBoundary, vector<Point>& polyline);
/*** Verify polygon ***/
bool verifyLengthSegPolygon(const vector<Point>& vecPolygon);
bool verifyAnglePolygon(const vector<Point>& vecPolygon);

int main(int argc, char** argv)
{
    /********** Parameters ********/
    po::options_description general_opt("Allowed options are: ");
    general_opt.add_options()
            ("help,h", "display this message")
            ("input,i", po::value<string>(), "input filename.")
            ("output,o", po::value<std::string>()->default_value("./"), "output dir (default ./).")
            ("regular,r","with quasi-regular verification.")
            ("eps,e","set output with eps format");

    bool parseOK=true;
    po::variables_map vm;
    try{
        po::store(po::parse_command_line(argc, argv, general_opt), vm);
    }catch(const exception& ex){
        trace.info()<< "Error checking program options: "<< ex.what()<< endl;
        parseOK=false;
    }

    po::notify(vm);
    if(vm.count("help")||argc<=1|| !parseOK || !vm.count("input") || !vm.count("output"))
    {
        trace.info()<< "Execution : " <<endl << "Options: "<<endl
                    << general_opt << "\n";
        return 0;
    }
    bool eps = vm.count("eps");
    bool reg = vm.count("regular");
    string inputFile=vm["input"].as<string>();
    string outputFile=vm["output"].as<string>();
    /********** Parameters ********/

    string filename;
    /********** Extract contour points ***************/
    getConnectedComponent(inputFile.c_str(),outputFile.c_str(),127,4,4);
    filename=outputFile+".txt";
    int width=0, height=0;
    int** tabLabels=readConnectedComponent(filename,width,height);
    vector<vector<Point> > vecConnectedComponent=getLabelPoints(tabLabels,width,height);
    //Extract boundaries
    vector<vector<Point> > aContour=getBoundary8C(tabLabels, width, height,vecConnectedComponent);
    if(aContour.size()==0) {
        cerr<<"Error aContour.size()=0"<<endl;
        exit(-1);
    }
    /********** Extract contour points ***************/

    /********** Verify isolated points ***********/
    if(aContour.size()>2){
        cerr<<"Error multiple objets in the image"<<endl;
        exit(-1);
    }
    for(size_t it_contour=0; it_contour<aContour.size(); it_contour++)
        if(aContour.at(it_contour).size()<4){
            cerr<<"Error triangular contour"<<endl;
            exit(-1);
        }
    /********** Verify isolated points ***********/

    /********** Compute convex hull ************/
    vector<vector<Point> > vecConvexhull;
    for(size_t it_contour=0; it_contour<aContour.size(); it_contour++) {
        vector<Point> ch=computeConvexHull(aContour.at(it_contour));
        vecConvexhull.push_back(ch);
    }
    /********** Compute convex hull ************/

    /********** Extract polygon from polyline ************/
    vector<vector<Point> > vecPolygon;
    for(size_t it_contour=0; it_contour<aContour.size(); it_contour++) {
        vector<Point> poly=getPolygon(tabLabels, aContour.at(it_contour),vecConvexhull.at(it_contour));
        vecPolygon.push_back(poly);
    }
    ofstream outfile;
    outfile.open(outputFile.c_str());
    for(size_t it_contour=0; it_contour<aContour.size(); it_contour++) {
        for(vector<Point>::const_iterator it=vecPolygon.at(it_contour).begin(); it!=vecPolygon.at(it_contour).end(); it++)
            outfile<<(*it)[1]<<" "<<-(*it)[0]<<" ";
        outfile<<endl;
    }
    outfile.close();
    /********** Extract polygon from polyline ************/

    /********** Verify angle + length polygon ***********/
    if(reg)
        for(size_t it_contour=0; it_contour<aContour.size(); it_contour++) {
            bool isLength=verifyLengthSegPolygon(vecPolygon.at(it_contour));
            bool isAngle=verifyAnglePolygon(vecPolygon.at(it_contour));
            if(!isLength || !isAngle) {
                cerr<<"Polygon is not quasi regular : "<<isLength<<" and "<<isAngle<<endl;
                exit(-1);
            }
        }
    /********** Verify angle + length polygon **********/

    //Remove temporary files of connected component
    filename="rm "+outputFile+".txt";
    system(filename.c_str());

    return 0;
}

/*** Read connected component file ***/
int** readConnectedComponent(string filename, int &width, int &height)
{
    int** labelOfPixel=NULL;
    ifstream myfile (filename.c_str());
    if (myfile.is_open()) {
        myfile >> width >> height;
        //read table of labels
        labelOfPixel = new int *[width];
        for (int i = 0; i < width; ++i)
        {
            labelOfPixel[i] = new int[height];
            for (int j = 0; j < height; j++)
                myfile >> labelOfPixel[i][j];
        }
    }
    return labelOfPixel;
}

vector<int> getLabelVector(int** tabLabels, int width, int height) {
    vector<int> vecLabels;
    int lab;
    for(int i=0; i<width; i++)
        for(int j=0; j<height; j++) {
            lab=tabLabels[i][j];
            if(!isExist(vecLabels,lab))
                vecLabels.push_back(lab);
        }
    return vecLabels;
}

vector<vector<Point> > getLabelPoints(int** tabLabels, int width, int height) {
    vector<int> vecLabels=getLabelVector(tabLabels,width,height);
    vector<vector<Point> > vecPoints;
    int curLab;
    for(vector<int>::const_iterator it=vecLabels.begin(); it!=vecLabels.end(); it++)
    {
        vector<Point> pts;
        for(int i=0; i<width; i++)
            for(int j=0; j<height; j++) {
                curLab=*it;
                if(tabLabels[i][j]==curLab)
                    pts.push_back(Point(i,j));
            }
        vecPoints.push_back(pts);
    }
    return vecPoints;
}
/*** Read connected component file ***/

/*** Extract boundary : http://users.utcluj.ro/~rdanescu/PI-L6e.pdf ***/
vector<Point> getNeighbourPoints(Point currentPoint, int connectiviy=4) {
    vector<Point> neighbors;
    neighbors.push_back(Point(currentPoint[0] + 1, currentPoint[1]));
    if(connectiviy==8)
        neighbors.push_back(Point(currentPoint[0] + 1, currentPoint[1] + 1));
    neighbors.push_back(Point(currentPoint[0], currentPoint[1] + 1));
    if(connectiviy==8)
        neighbors.push_back(Point(currentPoint[0] - 1, currentPoint[1] + 1));
    neighbors.push_back(Point(currentPoint[0] - 1, currentPoint[1]));
    if(connectiviy==8)
        neighbors.push_back(Point(currentPoint[0] - 1, currentPoint[1] - 1));
    neighbors.push_back(Point(currentPoint[0], currentPoint[1] - 1));
    if(connectiviy==8)
        neighbors.push_back(Point(currentPoint[0] + 1, currentPoint[1] - 1));
    return neighbors;
}

Point getNextBounderyPoint(int** tabLabels, int width, int height, int& dir, Point currentPoint, int connectiviy=4) {
    vector<Point> neighbors;
    Point p;
    int curLabel=tabLabels[currentPoint[0]][currentPoint[1]];
    if(connectiviy==4) {
        dir = (dir + 3) % 4;
        neighbors=getNeighbourPoints(currentPoint,connectiviy);
        while(true) {
            if (neighbors.at(dir)[0]>=0 && neighbors.at(dir)[0]<width
                    && neighbors.at(dir)[1]>=0 && neighbors.at(dir)[1]<height) {
                if(tabLabels[neighbors.at(dir)[0]][neighbors.at(dir)[1]]!=curLabel) {
                    if(dir < 3) dir++;
                    else dir = 0;
                }
                else return neighbors.at(dir);
            }
            else {
                if(dir < 3) dir++;
                else dir = 0;
            }
        }
    }
    else { //connectiviy==8
        if(dir % 2 == 0)  dir = (dir + 7) % 8;
        else dir = (dir + 6) % 8;
        neighbors=getNeighbourPoints(currentPoint,connectiviy);
        while(true) {
            if (neighbors.at(dir)[0]>=0 && neighbors.at(dir)[0]<width
                    && neighbors.at(dir)[1]>=0 && neighbors.at(dir)[1]<height) {
                if(tabLabels[neighbors.at(dir)[0]][neighbors.at(dir)[1]]!=curLabel) {
                    if(dir < 7) dir++;
                    else dir = 0;
                }
                else return neighbors.at(dir);
            }
            else {
                if(dir < 7) dir++;
                else dir = 0;
            }
        }
    }
    return p;
}

vector<Point> getBoundary(int** tabLabels, int width, int height, Point p, int connectivity=4) {
    Point currentPoint = p;
    vector<Point> boundary;
    int dir = 0;
    if(connectivity==8)
        dir = 7;
    currentPoint = getNextBounderyPoint(tabLabels, width, height, dir, currentPoint,connectivity);
    boundary.push_back(currentPoint);
    while(currentPoint != p) {
        currentPoint = getNextBounderyPoint(tabLabels, width, height, dir, currentPoint,connectivity);
        boundary.push_back(currentPoint);
    }
    return boundary;
}

vector<vector<Point> > getBoundary4C(int** tabLabels, int width, int height, vector<vector<Point> >& vecPts) {
    vector<vector<Point> > vecBoundary;
    for(size_t id=0; id<vecPts.size(); id++) {
        vector<Point> boundary = getBoundary(tabLabels, width, height, vecPts.at(id).front(),4);
        vecBoundary.push_back(boundary);
    }
    return vecBoundary;
}

vector<vector<Point> > getBoundary8C(int** tabLabels, int width, int height, vector<vector<Point> >& vecPts) {
    vector<vector<Point> > vecBoundary;
    for(size_t id=0; id<vecPts.size(); id++) {
        vector<Point> boundary = getBoundary(tabLabels, width, height, vecPts.at(id).front(),8);
        vecBoundary.push_back(boundary);
    }
    return vecBoundary;
}
/*** Extract boundary ***/

/*** Convex hull computation ***/
vector<Point> computeConvexHull(const vector<Point>& vecPoint)
{
    /********** Construct convex hull of contour ***************/
    typedef InHalfPlaneBySimple3x3Matrix<Point, DGtal::int64_t> Functor;
    Functor functor;
    vector<Point> convexhull;

    //typedef PredicateFromOrientationFunctor2<Functor, true, false> StrictPredicate; //clockwise order
    typedef PredicateFromOrientationFunctor2<Functor, false, false> StrictPredicate; //counter-clockwise order
    StrictPredicate predicate( functor );
    functions::Hull2D::andrewConvexHullAlgorithm( vecPoint.begin(), vecPoint.end(), back_inserter(convexhull), predicate);
    /********** Construct convex hull of contour ***************/

    //verification for the last and first segments of redundance
    if(convexhull.size()>2) {
        //First segment
        Point p1=convexhull.front();
        Point p2=convexhull.at(1);
        int dx=p2[0]-p1[0];
        int dy=p2[1]-p1[1];
        int a1=dy;
        int b1=-dx;
        int c1=a1*p1[0]+b1*p1[1];
        int p=pgcd(a1,b1);
        if(p!=1) {
            a1=a1/p;
            b1=b1/p;
            c1=c1/p;
        }
        p1=convexhull.back();
        p2=convexhull.front();
        dx=p2[0]-p1[0];
        dy=p2[1]-p1[1];
        int a2=dy;
        int b2=-dx;
        int c2=a2*p1[0]+b2*p1[1];
        p=pgcd(a2,b2);
        if(p!=1) {
            a2=a2/p;
            b2=b2/p;
            c2=c2/p;
        }
        if((a1==a2)&&(b1==b2)&&(c1==c2))
            convexhull.erase(convexhull.begin());
    }
    if(convexhull.size()>2) {
        //Last segment
        Point p1=convexhull.back();
        Point p2=convexhull.at(convexhull.size()-2);
        int dx=p2[0]-p1[0];
        int dy=p2[1]-p1[1];
        int a1=dy;
        int b1=-dx;
        int c1=a1*p1[0]+b1*p1[1];
        int p=pgcd(a1,b1);
        if(p!=1) {
            a1=a1/p;
            b1=b1/p;
            c1=c1/p;
        }
        p1=convexhull.front();
        p2=convexhull.back();
        dx=p2[0]-p1[0];
        dy=p2[1]-p1[1];
        int a2=dy;
        int b2=-dx;
        int c2=a2*p1[0]+b2*p1[1];
        p=pgcd(a2,b2);
        if(p!=1) {
            a2=a2/p;
            b2=b2/p;
            c2=c2/p;
        }

        if((a1==a2)&&(b1==b2)&&(c1==c2))
            convexhull.pop_back();
    }
    return convexhull;
}
/*** Convex hull computation ***/

/*** Polygon extraction ***/
bool testLinePolygon(int** tabLabels, Point startPoint, Point endPoint, vector<Point> &boundery8, int startIndexBoundery, int endIndexBoundery) {
    int labelCurrent = tabLabels[startPoint[0]][startPoint[1]];
    //Vertial segment
    if(startPoint[0] == endPoint[0])
    {
        int x = startPoint[0];
        int yStart, yEnd;
        yStart = startPoint[1] < endPoint[1] ? startPoint[1] : endPoint[1];//getMinInteger(startPoint[1], endPoint[1]);
        yEnd = startPoint[1] > endPoint[1] ? startPoint[1] : endPoint[1];//getMaxInteger(startPoint[1], endPoint[1]);

        //Verify the existence of points do not have the same label as startPoint and endPoint belonging to the seg
        for(int y = yStart; y <= yEnd; y++)
            if(tabLabels[x][y] != labelCurrent)
                return false;

        //Verify the existence of any boundary point on the right side of the seg
        for(int i = startIndexBoundery + 1; i < endIndexBoundery; i++) {
            Point p = boundery8.at(i);
            int x = p[0], y = p[1];

            if(startPoint[1] < endPoint[1]) {
                if(x > startPoint[0])
                    return false;
            }
            else {
                if(x < startPoint[0])
                    return false;
            }
        }
        return true;
    }
    else
    {
        int xStart = startPoint[0] < endPoint[0] ? startPoint[0] : endPoint[0];//getMinInteger(startPoint[0], endPoint[0]);
        int xEnd = startPoint[0] >endPoint[0]  ? startPoint[0] : endPoint[0];//getMaxInteger(startPoint[0], endPoint[0]);
        double slope = (double)(startPoint[1] - endPoint[1]) / (startPoint[0] - endPoint[0]);

        //Verify the existence of points do not have the same label as startPoint and endPoint belonging to the seg
        if(abs(slope) <= 1) {
            for(int x = xStart; x <= xEnd; x++) {
                int y;

                if(startPoint[0] < endPoint[0])
                    y = ceil(slope * x - slope * startPoint[0] + startPoint[1] - DELTA);
                else
                    y = floor(slope * x - slope * startPoint[0] + startPoint[1] + DELTA);

                if(tabLabels[x][y] != labelCurrent)
                    return false;
            }
        }
        else {
            int yStart = startPoint[1] < endPoint[1] ? startPoint[1] : endPoint[1];//getMinInteger(startPoint[1], endPoint[1]);
            int yEnd = startPoint[1] > endPoint[1] ? startPoint[1] : endPoint[1];//getMaxInteger(startPoint[1], endPoint[1]);
            double invSlope = (double)(startPoint[0] - endPoint[0]) / (startPoint[1] - endPoint[1]);

            for(int y = yStart; y <= yEnd; y++) {
                int x;
                if(startPoint[1] < endPoint[1])
                    x = floor(invSlope * y - invSlope * startPoint[1] + startPoint[0] + DELTA);
                else
                    x = ceil(invSlope * y - invSlope * startPoint[1] + startPoint[0] - DELTA);

                if(tabLabels[x][y] != labelCurrent)
                    return false;
            }
        }
        //Verify the existence of any boundary point on the right side of the seg
        for(int i = startIndexBoundery + 1; i < endIndexBoundery; i++)
        {
            Point p = boundery8.at(i);
            int x = p[0], y = p[1];

            if(startPoint[0] < endPoint[0])
            {
                if((slope * x - slope * startPoint[0] + startPoint[1] - y) > 0)
                    return false;
            }
            else
            {
                if((slope * x - slope * startPoint[0] + startPoint[1] - y) < 0)
                    return false;
            }
        }
        return true;
    }
}

bool isConcavePart(int** tabLabels, Point startPoint, Point endPoint) {
    int labelCurrent = tabLabels[startPoint[0]][startPoint[1]];
    bool onHullUp = startPoint[0] > endPoint[0];
    if(startPoint[0] == endPoint[0]) {
        int x = startPoint[0];
        int yStart, yEnd;
        yStart = startPoint[1] < endPoint[1] ? startPoint[1] : endPoint[1];//getMinInteger(startPoint[1], endPoint[1]);
        yEnd = startPoint[1] > endPoint[1] ? startPoint[1] : endPoint[1];//getMaxInteger(startPoint[1], endPoint[1]);
        for(int y = yStart; y <= yEnd; y++) {
            if(tabLabels[x][y] != labelCurrent)
                return true;
        }
        return false;
    }
    else
    {
        int xStart = startPoint[0] < endPoint[0] ? startPoint[0] : endPoint[0];//getMinInteger(startPoint[0], endPoint[0]);
        int xEnd = startPoint[0] >endPoint[0]  ? startPoint[0] : endPoint[0];//getMaxInteger(startPoint[0], endPoint[0]);
        double slope = (double)(startPoint[1] - endPoint[1]) / (startPoint[0] - endPoint[0]);
        if(abs(slope) <= 1) {
            for(int x = xStart; x <= xEnd; x++) {
                int y;
                if(onHullUp)
                    y = floor(slope * x - slope * startPoint[0] + startPoint[1] + DELTA);
                else
                    y = ceil(slope * x - slope * startPoint[0] + startPoint[1] - DELTA);

                if(tabLabels[x][y] != labelCurrent)
                    return true;
            }
            return false;
        }
        else {
            int yStart = startPoint[1] < endPoint[1] ? startPoint[1] : endPoint[1];//getMinInteger(startPoint[1], endPoint[1]);
            int yEnd = startPoint[1] > endPoint[1] ? startPoint[1] : endPoint[1];//getMaxInteger(startPoint[1], endPoint[1]);
            double invSlope = (double)(startPoint[0] - endPoint[0]) / (startPoint[1] - endPoint[1]);
            for(int y = yStart; y <= yEnd; y++) {
                int x;
                if((onHullUp == true && invSlope < 0) || (onHullUp == false && invSlope > 0))
                    x = floor(invSlope * y - invSlope * startPoint[1] + startPoint[0] + DELTA);
                else if ((onHullUp == true && invSlope > 0) || (onHullUp == false && invSlope < 0))
                    x = ceil(invSlope * y - invSlope * startPoint[1] + startPoint[0] - DELTA);

                if(tabLabels[x][y] != labelCurrent)
                    return true;
            }
            return false;
        }
    }
}

vector<Point> getPolygonInConcavePart(int** tabLabels, vector<Point>& boundery8, Point startPoint, Point endPoint) {
    int indexBoundery;
    //Find index of the startPoint of concave part in boundary seq. points
    for(indexBoundery = 0; indexBoundery < boundery8.size(); indexBoundery++)
        if(boundery8.at(indexBoundery) == startPoint)
            break;
    vector<Point> polygon;
    polygon.push_back(startPoint);
    Point nextPoint;
    int startIndexBoundery = indexBoundery;

    for(int index = indexBoundery + 1; ; index++)
    {
        //justify index if it is out of seq. boundary
        if(index >= boundery8.size() - 1)
            index = 0;
        if(startIndexBoundery >= boundery8.size() - 1 || startIndexBoundery < 0)
            startIndexBoundery = 0;

        //Verify seg (startPoint, nextpoint (a boundary point)) is max dist
        if(testLinePolygon(tabLabels, polygon.at(polygon.size() - 1), boundery8.at(index), boundery8, startIndexBoundery, index))
            nextPoint = boundery8.at(index);
        else {
            startIndexBoundery = index - 1;
            index--;
            polygon.push_back(nextPoint);
        }

        if(boundery8.at(index) == endPoint)
            break;
    }

    return polygon;
}

vector<Point> getPolygon(int** tabLabels, vector<Point>& vecBoundary, vector<Point>& polyline) {
    vector<Point> polygon;
    for(size_t i=0; i<polyline.size()-1; i++) {
        if(isConcavePart(tabLabels, polyline.at(i), polyline.at(i + 1))) { //if concave, build seg polygon
            vector<Point> poly=getPolygonInConcavePart(tabLabels, vecBoundary, polyline.at(i), polyline.at(i + 1));
            for(size_t it = 0; it < poly.size(); it++)
                polygon.push_back(poly.at(it));
        }
        else //if convex then push ft point hull
            polygon.push_back(polyline.at(i));
    }
    //Last segment
    if(isConcavePart(tabLabels, polyline.back(), polyline.front())) { //if concave, build seg polygon
        vector<Point> poly=getPolygonInConcavePart(tabLabels, vecBoundary, polyline.back(), polyline.front());
        for(size_t it = 0; it < poly.size(); it++)
            polygon.push_back(poly.at(it));
    }
    else //if convex then push ft point hull
        polygon.push_back(polyline.back());

    return polygon;
}
/*** Polygon extraction ***/

/*** Verify polygon ***/
bool verifyLengthSegPolygon(const vector<Point>& vecPolygon) {
    for(vector<Point>::const_iterator it=vecPolygon.begin(); it+1!=vecPolygon.end(); it++)
        if(distancePoints(*it,*(it+1))<2)//*sqrt(2)
            return false;
    //last segment
    if(distancePoints(vecPolygon.front(),vecPolygon.back())<2*sqrt(2))
        return false;
    return true;
}

bool verifyAnglePolygon(const vector<Point>& vecPolygon){
    //first angle
    double angle=acuteAngle(vecPolygon.back(),vecPolygon.front(),vecPolygon.at(1));
    if(angle<M_PI/2.0)
        return false;
    for(vector<Point>::const_iterator it=vecPolygon.begin()+1; it+1!=vecPolygon.end(); it++) {
        angle=acuteAngle(*(it-1),*it,*(it+1));
        if(angle<M_PI/2.0)
            return false;
    }
    //last angle
    angle=acuteAngle(vecPolygon.at(vecPolygon.size()-2),vecPolygon.back(),vecPolygon.front());
    if(distancePoints(vecPolygon.front(),vecPolygon.back())<2*sqrt(2))
        return false;
    return true;
}

/*** Verify polygon ***/
