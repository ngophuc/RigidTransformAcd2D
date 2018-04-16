#include <DGtal/io/readers/PointListReader.h>
#include "DGtal/io/colormaps/HueShadeColorMap.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "Halfplane.h"
#include "Boundingbox.h"
#include "RigidTransform.h"
#include "definition.h"

namespace po=boost::program_options;
using namespace std;

template <typename TPoint>
bool isCounterClockWise(const vector<TPoint> & aCurve);
vector<Point> computeConvexHull(const vector<Point>& vecPoints);
vector<Point> generateConvexShape(const vector<Point>& vecPoints, vector<HalfPlane>& vecHP, vector<Point> &vecVertices, vector<Point> &vecFacets, bool isVerbose=false);
vector<vector<Point> > generateConvexShape(const vector<vector<Point> >& vecPoints, vector<vector<HalfPlane> > &vHP, vector<vector<Point> > &vVertices, vector<vector<Point> > &vFacets, bool isVerbose);

int main(int argc, char** argv) {
    /********** Parameters ********/
    po::options_description general_opt("Allowed options are: ");
    general_opt.add_options()
            ("help,h", "display this message")
            ("input,i", po::value<string>(), "input filename.")
            ("output,o", po::value<std::string>()->default_value("./"), "output dir (default ./).")
            ("tx,a", po::value<double>()->default_value(0.0), "x-translation.")
            ("ty,b", po::value<double>()->default_value(0.0), "y-translation.")
            ("theta,t", po::value<double>()->default_value(0.0), "rotation angle.")
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
    bool paramTransf=vm.count("tx") && vm.count("ty") && vm.count("theta");
    double a=0.0, b=0.0, theta=0.0;
    if (paramTransf) {
        a=vm["tx"].as<double>();
        b=vm["ty"].as<double>();
        theta=vm["theta"].as<double>();
    }
    string inputFile=vm["input"].as<string>();
    string outputFile=vm["output"].as<string>();
    string infile = inputFile.substr(0, inputFile.find_last_of("."));
    cout<<"inputFile="<<inputFile<<" infile="<<infile<<" outputFile="<<outputFile<<endl;
    /********** Parameters ********/

    /*** Polygonalize pgm image ***/
    char instruction[5*FILENAMESIZE];
    char filename[FILENAMESIZE];
    sprintf(filename,"%s.sdp",infile.c_str());
    //-i *.pgm -o *_poly.sdp
    sprintf(instruction,"./polygonalization -i %s -o %s",inputFile.c_str(),filename);
    if(reg)
        sprintf(instruction,"%s -r",instruction);
    if(eps)
        sprintf(instruction,"%s -e",instruction);
    system(instruction);
    /*** Polygonalize pgm image ***/

    /*** Convert sdp file to poly file ***/
    vector< vector<Point> > vPoints = PointListReader<Point>::getPolygonsFromFile(filename);
    if(vPoints.size()>2) {
        cout<<"Multi polylines ! (comming soon)"<<endl;
        exit(-1);
    }
    sprintf(filename,"%s.poly",infile.c_str());
    ofstream outFile;
    outFile.open (filename);
    outFile<<vPoints.size()-1<<endl;//number of polygonal chains (without background)
    outFile<<vPoints.back().size()<<" out"<<endl;//first poly chain with nb of points and out chain
    //for(size_t id=0; id<vPoints.size(); id++)
    size_t id=1;
    {
        //coordinate of points
        for(auto it=vPoints.at(id).begin(); it!=vPoints.at(id).end();it++)
            outFile<<(*it)[0]<<" "<<(*it)[1]<<endl;
        //the order of the polychain vertices, must be ccw
        if(isCounterClockWise(vPoints.at(id)))
            for(size_t it=1; it<=vPoints.at(id).size();it++)
                outFile<<it<<" ";
        else
            for(size_t it=vPoints.at(id).size(); it>0;it--)
                outFile<<it<<" ";
        outFile<<endl;
    }
    outFile.close();
    /*** Convert sdp file to poly file ***/

    /*** Use Acd2d decomposition ***/
    //input : *.poly (ex: BarbedThing.poly)
    //output : *-acd.poly (ex: BarbedThing-acd.poly) and *_0.sdp, *_1.sdp, ...
    sprintf(filename,"%s.poly",infile.c_str());
    sprintf(instruction,"./decomposeShapeAcd2d -s %s",filename);
    system(instruction);

    ifstream inFile;
    sprintf(filename,"%s.txt",infile.c_str());
    inFile.open(filename);
    int nbFile;
    if (inFile.is_open()) {
        inFile >> nbFile;
        inFile.close();
    }
    else {
        cout << "Unable to open file";
        exit(-1);
    }
    /*** Use Acd2d decomposition ***/

    /*** Read decomposition points ***/
    vector< vector< Point > > vecPoly;
    for(int it_File=0; it_File<nbFile; it_File++) {
        sprintf(filename,"%s_%d.sdp",infile.c_str(),it_File);
        vector< Point> poly = PointListReader<Point>::getPointsFromFile(filename);
        vecPoly.push_back(poly);
    }
    /*** Read decomposition points ***/

    /**** Draw decomposition *****/
    Board2D aBoard;
    aBoard.setLineWidth(15.0);
    HueShadeColorMap<double> hueMap(0.0,vecPoly.size());
    for(size_t id=0; id<vecPoly.size(); id++) {
        aBoard.setPenColor(hueMap(id));
        for(auto it=vecPoly.at(id).begin(); it+1!=vecPoly.at(id).end();it++)
            aBoard.drawLine((*it)[0],(*it)[1],(*(it+1))[0],(*(it+1))[1]);
        aBoard.drawLine(vecPoly.at(id).front()[0],vecPoly.at(id).front()[1],vecPoly.at(id).back()[0],vecPoly.at(id).back()[1]);
    }
    if(eps){
        sprintf(filename,"%s_decomp.eps",infile.c_str());
        aBoard.saveEPS(filename);
    }
    else{
        sprintf(filename,"%s_decomp.svg",infile.c_str());
        aBoard.saveSVG(filename);
    }
    aBoard.clear();
    /**** Draw decomposition *****/

    /**** Digitize decomposition ****/
    vector< vector< HalfPlane > > vecHP;
    vector< vector< Point > > vecFacets, vecVertices;
    vector< vector< Point> > vecConvexShape;
    vecConvexShape=generateConvexShape(vecPoly,vecHP,vecVertices,vecFacets,false);
    /**** Digitize decomposition ****/

    /**** Draw digitized sets *****/
    for(int id=0; id<vecConvexShape.size(); id++) {
        //show points
        aBoard << CustomStyle("PointVector",new  CustomColors(hueMap(id),hueMap(id)));
        for (size_t i=0; i<vecConvexShape.at(id).size(); i++) {
            Point p (vecConvexShape.at(id)[i][0],vecConvexShape.at(id)[i][1]);
            aBoard << p;
        }
    }
    if(eps){
        sprintf(filename,"%s_shape.eps",infile.c_str());
        aBoard.saveEPS(filename);
    }
    else{
        sprintf(filename,"%s_shape.svg",infile.c_str());
        aBoard.saveSVG(filename);
    }
    aBoard.clear();
    /**** Draw digitized sets *****/

    /**** Transform shape ****/
    transformation T;
    T[0]=a;
    T[1]=b;
    T[2]=theta;
    vector<vector<RealPoint> > tvecPolygon;
    vector<vector<Point> > tPtsPoly=transformPolygon(vPoints, tvecPolygon, T);
    Point pMin,pMax;
    int marge=10;
    id=1;
    findBoundingBox(tPtsPoly.at(id),pMin,pMax);
    Image imgOut(Domain(pMin-Point(marge,marge),pMax+Point(marge,marge)));
    for (vector<Point>::const_iterator it = tPtsPoly.at(id).begin(); it != tPtsPoly.at(id).end(); it++)
        imgOut.setValue(*it,OBJ);
    PGMWriter<Image>::exportPGM(outputFile,imgOut);
    /**** Transform shape ****/

    //Remove temporary files
    sprintf(filename,"rm %s.txt",infile.c_str());
    system(filename);
    for(int it_File=0; it_File<nbFile; it_File++) {
        sprintf(filename,"rm %s_%d.sdp",infile.c_str(),it_File);
        system(filename);
    }
    sprintf(filename,"rm %s.sdp",infile.c_str());
    system(filename);
    sprintf(filename,"rm %s.poly",infile.c_str());
    system(filename);
    sprintf(filename,"rm %s-acd.poly",infile.c_str());
    system(filename);
    cerr<<"Well done !"<<endl;
    return 0;
}

template <typename TPoint>
bool isCounterClockWise(const vector<TPoint> & aCurve)
{
    double sum=0.0;
    TPoint p0 = *(aCurve.begin());
    typename vector<TPoint>::const_iterator itContour = aCurve.begin();
    itContour++;
    TPoint p=*itContour;
    itContour++;
    for( ; itContour!=aCurve.end(); itContour++)
    {
        TPoint pSuiv = *itContour;
        typename TPoint::Coordinate v1x = p[0] - p0[0];
        typename TPoint::Coordinate v1y = p[1] - p0[1];
        typename TPoint::Coordinate v2x = pSuiv[0] - p[0];
        typename TPoint::Coordinate v2y = pSuiv[1] - p[1];
        sum+=(v1x*v2y)-(v2x*v1y);
        p=pSuiv;
    }
    return (sum>0.0);
}

vector<Point> computeConvexHull(const vector<Point>& vecPoints)
{
    /********** Construct convex hull of contour ***************/
    typedef InHalfPlaneBySimple3x3Matrix<Point, DGtal::int64_t> Functor;
    Functor functor;
    vector<Point> convexhull;
    DGtal::MelkmanConvexHull<Z2i::Point, Functor> ch( functor );
    for (vector<Point>::const_iterator it=vecPoints.begin(); it != vecPoints.end(); ++it)
        ch.add(*it);
    functions::Hull2D::melkmanConvexHullAlgorithm(vecPoints.begin(), vecPoints.end(), back_inserter(convexhull), functor );
    //verification for the last and first segments of redundance
    if(convexhull.size()>3)
    {
        //First segment
        Point p1=convexhull.front();
        Point p2=convexhull.at(1);
        int dx=p2[0]-p1[0];
        int dy=p2[1]-p1[1];
        int a1=dy;
        int b1=-dx;
        int c1=a1*p1[0]+b1*p1[1];
        int p=pgcd(a1,b1);
        if(p!=1)
        {
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
        if(p!=1)
        {
            a2=a2/p;
            b2=b2/p;
            c2=c2/p;
        }
        if((a1==a2)&&(b1==b2)&&(c1==c2))
            convexhull.erase(convexhull.begin());
        //cout<<"a1="<<a1<<" and b1="<<b1<<" and c1="<<c1<<" and a2="<<a2<<" and b2="<<b2<<" and c2="<<c2<<endl;
    }
    if(convexhull.size()>3)
    {
        //Last segment
        Point p1=convexhull.back();
        Point p2=convexhull.at(convexhull.size()-2);
        int dx=p2[0]-p1[0];
        int dy=p2[1]-p1[1];
        int a1=dy;
        int b1=-dx;
        int c1=a1*p1[0]+b1*p1[1];
        int p=pgcd(a1,b1);
        if(p!=1)
        {
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
        if(p!=1)
        {
            a2=a2/p;
            b2=b2/p;
            c2=c2/p;
        }

        if((a1==a2)&&(b1==b2)&&(c1==c2))
            convexhull.pop_back();
        //cout<<"a1="<<a1<<" and b1="<<b1<<" and c1="<<c1<<" and a2="<<a2<<" and b2="<<b2<<" and c2="<<c2<<endl;
    }
    /********** Construct convex hull of contour ***************/
    return convexhull;
}

vector<Point> generateConvexShape(const vector<Point>& vecPoints, vector<HalfPlane>& vecHP, vector<Point> &vecVertices, vector<Point> &vecFacets, bool isVerbose)
{
    /******* calculate bounding box ********/
    BoundingBox BB=findBoundingBox(vecPoints);
    /******* calculate bounding box ********/

    /******* calculate convex hull ********/
    vector<Point> chull=computeConvexHull(vecPoints);
    /******* calculate convex hull ********/

    /******* vertices and facets of convex hull *******/
    for(auto it=chull.begin();it!=chull.end(); it++)
        vecVertices.push_back(*it);
    for(size_t it=0; it<vecVertices.size()-1; it++)
        vecFacets.push_back(Point(it,it+1));
    vecFacets.push_back(Point(vecVertices.size()-1,0));
    /******* vertices and facets of convex hull *******/

    /******* calculate halfplane from convex hull ******/
    RealPoint p=findCentroid(vecVertices);
    Point p1,p2;
    int a,b,c,sign,dx,dy;
    for(size_t i=0; i<vecFacets.size(); i++)
    {
        p1=vecVertices.at(vecFacets.at(i)[0]);
        p2=vecVertices.at(vecFacets.at(i)[1]);
        dx=p2[0]-p1[0];
        dy=p2[1]-p1[1];
        a=dy;
        b=-dx;
        c=-(a*p1[0]+b*p1[1]);
        sign=((a*p[0]+b*p[1]+c))>0 ? 1:-1;
        vecHP.push_back(HalfPlane(a,b,c,sign,p1,p2));
    }
    if(isVerbose)
    {
        cout<<"Number of halfplanes extracted="<<vecHP.size()<<endl;
        cout<<"centroid="<<p<<"p1="<<p[1]<<endl;
        for(size_t it=0; it<vecHP.size(); it++)
            cout<<"("<<vecHP.at(it).getA()<<","<<vecHP.at(it).getB()<<","<<vecHP.at(it).getC()<<","<<vecHP.at(it).getSign()<<")"<<endl;
    }
    /******* calculate halfplane from convex hull ******/

    /******* generate convexe objet from convex hull ******/
    Point min=BB.getPointMin();
    Point max=BB.getPointMax();
    vector<Point> vecPoint_CH;
    bool isGood;
    for(int i=min[0]; i<=max[0]; i++)//for(int i=-20; i<=20; i++)
        for(int j=min[1]; j<=max[1]; j++)//for(int j=-20; j<=20; j++)
        {
            isGood=true;
            Point p(i,j);
            for(size_t it=0; it<vecHP.size(); it++)//
                if(!vecHP.at(it).isBelong(p))
                    isGood=false;
            if(isGood)
                //vecPoint_CH.push_back(p-min);
                vecPoint_CH.push_back(p);
        }
    if(isVerbose)
        cout << vecPoint_CH.size() << " points in CH" << endl;
    /******* generate convexe objet from convex hull ******/
    return vecPoint_CH;
}

vector<vector<Point> > generateConvexShape(const vector<vector<Point> >& vecPoints, vector<vector<HalfPlane> >& vHP, vector<vector<Point> > &vVertices, vector<vector<Point> > &vFacets, bool isVerbose)
{
    vector<vector<Point> > vPoint_CH;
    for (size_t it=0; it<vecPoints.size(); it++)
    {
        vector<HalfPlane> vecHP;
        vector<Point> vecVertices;
        vector<Point> vecFacets;
        vector<Point> vecPoint_CH=generateConvexShape(vecPoints.at(it),vecHP,vecVertices,vecFacets,isVerbose);
        vHP.push_back(vecHP);
        vVertices.push_back(vecVertices);
        vFacets.push_back(vecFacets);
        vPoint_CH.push_back(vecPoint_CH);
    }
    return vPoint_CH;
}
