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

vector<vector<Point> > getTriangles(const vector<Point>& vecPoints, const vector<vector<Point> >& vecPoly);

int main(int argc, char** argv) {
    /********** Parameters ********/
    po::options_description general_opt("Allowed options are: ");
    general_opt.add_options()
            ("help,h", "display this message")
            ("input,i", po::value<string>(), "input filename.")
            ("output,o", po::value<std::string>()->default_value("./"), "output dir (default ./).")
            ("dir,d", po::value<std::string>()->default_value("./"), "execute dir (default ./).")
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
    //bool paramTransf=vm.count("tx") || vm.count("ty") || vm.count("theta");
    double a=0.0, b=0.0, theta=0.0;
    //if (paramTransf) {
        a=vm["tx"].as<double>();
        b=vm["ty"].as<double>();
        theta=vm["theta"].as<double>();
    //}
    string dir=vm["dir"].as<string>();
    string inputFile=vm["input"].as<string>();
    string outputFile=vm["output"].as<string>();
    string infile = inputFile.substr(0, inputFile.find_last_of("."));
    //cout<<"inputFile="<<inputFile<<" infile="<<infile<<" outputFile="<<outputFile<<endl;
    /********** Parameters ********/

    /*** Polygonalize pgm image ***/
    char instruction[5*FILENAMESIZE];
    char filename[FILENAMESIZE];
    sprintf(filename,"%s.sdp",infile.c_str());
    //-i *.pgm -o *_poly.sdp
    sprintf(instruction,"%spolygonalization -i %s -o %s",dir.c_str(),inputFile.c_str(),filename);
    if(reg)
        sprintf(instruction,"%s -r",instruction);
    if(eps)
        sprintf(instruction,"%s -e",instruction);
    cout<<"instruction: "<<instruction<<endl;
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
    {
        size_t id=1;
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

    /**** Draw polygon *****/
    Board2D aBoard;
    aBoard.setLineWidth(15.0);
    aBoard.setPenColor(DGtal::Color::Black);
    for(size_t id=0; id<vPoints.size(); id++)
    {
        for(auto it=vPoints.at(id).begin(); it+1!=vPoints.at(id).end();it++)
            aBoard.drawLine((*it)[0],(*it)[1],(*(it+1))[0],(*(it+1))[1]);
        aBoard.drawLine(vPoints.at(id).front()[0],vPoints.at(id).front()[1],vPoints.at(id).back()[0],vPoints.at(id).back()[1]);
    }
    if(eps){
        sprintf(filename,"%s_poly.eps",infile.c_str());
        aBoard.saveEPS(filename);
    }
    else{
        sprintf(filename,"%s_poly.svg",infile.c_str());
        aBoard.saveSVG(filename);
    }
    aBoard.clear();
    /**** Draw polygon *****/

    /*** Use Acd2d decomposition ***/
    //input : *.poly (ex: BarbedThing.poly)
    //output : *-acd.poly (ex: BarbedThing-acd.poly) and *.txt, *_0.sdp, *_1.sdp, ...
    sprintf(filename,"%s.poly",infile.c_str());
    sprintf(instruction,"%sdecomposeShapeAcd2d -s %s",dir.c_str(),filename);
    cout<<"instruction: "<<instruction<<endl;
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
    vector<vector<Point> > vTri=getTriangles(vPoints.at(1),vecPoly);//size_t id=1;
    for(int id_T=0; id_T<vTri.size(); id_T++)
        vecPoly.push_back(vTri.at(id_T));
    /*** Read decomposition points ***/

    /**** Draw decomposition *****/
    aBoard.setLineWidth(15.0);
    HueShadeColorMap<double> hueMap(0.0,vecPoly.size());
    for(size_t id=0; id<vecPoly.size(); id++) {
        aBoard.setPenColor(hueMap(id));
        for(auto it=vecPoly.at(id).begin(); it+1!=vecPoly.at(id).end();it++)
            aBoard.drawLine((*it)[0],(*it)[1],(*(it+1))[0],(*(it+1))[1]);
        aBoard.drawLine(vecPoly.at(id).front()[0],vecPoly.at(id).front()[1],vecPoly.at(id).back()[0],vecPoly.at(id).back()[1]);
    }
    //Draw polygon
    aBoard.setPenColor(DGtal::Color::Black);
    {
        size_t id=0;
        for(auto it=vPoints.at(id).begin(); it+1!=vPoints.at(id).end();it++)
            aBoard.drawLine((*it)[0],(*it)[1],(*(it+1))[0],(*(it+1))[1]);
        aBoard.drawLine(vPoints.at(id).front()[0],vPoints.at(id).front()[1],vPoints.at(id).back()[0],vPoints.at(id).back()[1]);
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
    //show points
    //aBoard << SetMode("PointVector", "Both");
    for(int id=0; id<vecConvexShape.size(); id++) {
        aBoard << CustomStyle("PointVector",new  CustomColors(hueMap(id),hueMap(id)));
        for (size_t i=0; i<vecConvexShape.at(id).size(); i++) {
            Point p (vecConvexShape.at(id)[i][0],vecConvexShape.at(id)[i][1]);
            aBoard << p;
        }
    }
    //Draw polygon
    aBoard.setPenColor(DGtal::Color::Black);
    {
        size_t id=0;
        for(auto it=vPoints.at(id).begin(); it+1!=vPoints.at(id).end();it++)
            aBoard.drawLine((*it)[0],(*it)[1],(*(it+1))[0],(*(it+1))[1]);
        aBoard.drawLine(vPoints.at(id).front()[0],vPoints.at(id).front()[1],vPoints.at(id).back()[0],vPoints.at(id).back()[1]);
    }
    if(eps){
        sprintf(filename,"%s_shape.eps",infile.c_str());
        aBoard.saveEPS(filename);
    }
    else{
        sprintf(filename,"%s_shape.svg",infile.c_str());
        aBoard.saveSVG(filename);
    }
    //aBoard.clear();
    /**** Draw digitized sets *****/

    /**** Transform shape ****/
    transformation T;
    T[0]=a;
    T[1]=b;
    T[2]=theta;
    vector<vector<Point> > tvecPoly;
    vector<RealPoint> vP;
    for(size_t it_contour=0; it_contour<vecPoly.size(); it_contour++) {
        RealPoint p=findCentroid(vecPoly.at(it_contour));
        vP.push_back(p);
    }
    RealPoint c=findCentroid(vP);
    for(size_t id=0; id<vecPoly.size(); id++) {
        vector<Point> tPoly=transformPolygonRound(vecPoly.at(id), T, c);
        tvecPoly.push_back(tPoly);
    }
    /**** Transform shape ****/

    /**** Draw transformed decomposition *****/
    aBoard.setLineWidth(15.0);
    for(size_t id=0; id<tvecPoly.size(); id++) {
        aBoard.setPenColor(hueMap(id));
        for(auto it=tvecPoly.at(id).begin(); it+1!=tvecPoly.at(id).end();it++)
            aBoard.drawLine((*it)[0],(*it)[1],(*(it+1))[0],(*(it+1))[1]);
        aBoard.drawLine(tvecPoly.at(id).front()[0],tvecPoly.at(id).front()[1],tvecPoly.at(id).back()[0],tvecPoly.at(id).back()[1]);
    }
    aBoard.setPenColor(DGtal::Color::Black);
    {
        size_t id=0;
        for(auto it=vPoints.at(id).begin(); it+1!=vPoints.at(id).end();it++)
            aBoard.drawLine((*it)[0],(*it)[1],(*(it+1))[0],(*(it+1))[1]);
        aBoard.drawLine(vPoints.at(id).front()[0],vPoints.at(id).front()[1],vPoints.at(id).back()[0],vPoints.at(id).back()[1]);
    }
    if(eps){
        sprintf(filename,"%s_tdecomp.eps",infile.c_str());
        aBoard.saveEPS(filename);
    }
    else{
        sprintf(filename,"%s_tdecomp.svg",infile.c_str());
        aBoard.saveSVG(filename);
    }
    aBoard.clear();
    /**** Draw transformed decomposition *****/

    /**** Digitize transformed shape ****/
    vector< vector< HalfPlane > > tvecHP;
    vector< vector< Point > > tvecFacets, tvecVertices;
    vector< vector< Point> > tvecConvexShape;
    tvecConvexShape=generateConvexShape(tvecPoly,tvecHP,tvecVertices,tvecFacets,false);
    /**** Digitize transformed shape ****/

    /**** Draw transformed shape ****/
    aBoard << SetMode("PointVector", "Both");
    for(size_t id=0; id<tvecConvexShape.size(); id++) {
        aBoard << CustomStyle("PointVector",new  CustomColors(hueMap(id),hueMap(id)));
        for (size_t i=0; i<tvecConvexShape.at(id).size(); i++)
            aBoard << Point(tvecConvexShape.at(id).at(i)[0],tvecConvexShape.at(id).at(i)[1]);
    }
    aBoard.setPenColor(DGtal::Color::Black);
    {
        size_t id=0;
        for(auto it=vPoints.at(id).begin(); it+1!=vPoints.at(id).end();it++)
            aBoard.drawLine((*it)[0],(*it)[1],(*(it+1))[0],(*(it+1))[1]);
        aBoard.drawLine(vPoints.at(id).front()[0],vPoints.at(id).front()[1],vPoints.at(id).back()[0],vPoints.at(id).back()[1]);
    }
    if(eps){
        sprintf(filename,"%s_tshape.eps",infile.c_str());
        aBoard.saveEPS(filename);
    }
    else{
        sprintf(filename,"%s_tshape.svg",infile.c_str());
        aBoard.saveSVG(filename);
    }
    aBoard.clear();
    /**** Draw transformed shape ****/

    /**** Save transformed shape ****/
    Point pMin,pMax;
    int marge=10;
    findBoundingBox(tvecConvexShape,pMin,pMax);
    Image imgOut(Domain(pMin-Point(marge,marge),pMax+Point(marge,marge)));
    for(size_t id=0; id<tvecConvexShape.size(); id++)
        for (vector<Point>::const_iterator it = tvecConvexShape.at(id).begin(); it != tvecConvexShape.at(id).end(); it++)
            imgOut.setValue(*it,OBJ);
    PGMWriter<Image>::exportPGM(outputFile,imgOut);
    /**** Save transformed shape ****/

    /******* Remove temporay files ******/
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

    if(eps)
        sprintf(filename,"rm %s_tdecomp.eps",infile.c_str());
    else
        sprintf(filename,"rm %s_tdecomp.svg",infile.c_str());
    system(filename);
    if(eps)
        sprintf(filename,"rm %s_tshape.eps",infile.c_str());
    else
        sprintf(filename,"rm %s_tshape.svg",infile.c_str());
    system(filename);
    /******* Remove temporay files ******/

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

bool isBoundarySegment(const vector<Point>& vecBoundary, Point p1, Point p2)
{
    int id1=findElement(vecBoundary,p1);
    int id2=findElement(vecBoundary,p2);
    if(id1==-1 || id2==-1)
        return false;
    if((abs(id1-id2)==1))//|| (abs(id1-id2)==(vecBoundary.size()-1)
        return true;
    return false;
}

bool isShareSegment(const vector<Point>& vecPoly, Point p1, Point p2)
{
    bool id1=isExist(vecPoly,p1);
    bool id2=isExist(vecPoly,p2);
    if(id1 && id2)
        return true;
    return false;
}

bool isShareSegment(const vector<vector<Point> >& vecPoly, Point p1, Point p2, size_t notId)
{
    for(size_t id=0; id<vecPoly.size(); id++)
        if(id!=notId && isShareSegment(vecPoly.at(id),p1,p2))
            return true;
    return false;
}

bool existSegment(const vector<Point>& vecP1, const vector<Point>& vecP2, Point p1, Point p2) {
    //P1
    size_t id1=findElement(vecP1,p1);
    if(id1!=-1)
        if(vecP2.at(id1)==p2)
            return true;
    size_t id2=findElement(vecP2,p1);
    if(id2!=-1)
        if(vecP1.at(id2)==p2)
            return true;
    //P2
    id1=findElement(vecP1,p2);
    if(id1!=-1)
        if(vecP2.at(id1)==p1)
            return true;
    id2=findElement(vecP2,p2);
    if(id2!=-1)
        if(vecP1.at(id2)==p1)
            return true;
    return false;
}

bool existTriangle(const vector<vector<Point> >& tri, Point p1, Point p2, Point p3)
{
    for(size_t id=0; id<tri.size(); id++) {
        if(tri.at(id).at(0)==p1 && tri.at(id).at(1)==p2 && tri.at(id).at(2)==p3)//123
            return true;
        if(tri.at(id).at(0)==p2 && tri.at(id).at(1)==p1 && tri.at(id).at(2)==p3)//213
            return true;
        if(tri.at(id).at(0)==p3 && tri.at(id).at(1)==p2 && tri.at(id).at(2)==p1)//321
            return true;
        if(tri.at(id).at(0)==p1 && tri.at(id).at(1)==p3 && tri.at(id).at(2)==p2)//132
            return true;
    }
    return false;
}

vector<vector<Point> > getTriangles(const vector<Point>& vecP1, const vector<Point>& vecP2)
{
    vector<vector<Point> > vecTriangles;
    for(size_t id=0; id<vecP1.size()-1; id++) {
        vector<Point> triangle;
        Point p1=vecP1.at(id);
        Point p2=vecP2.at(id);
        triangle.push_back(p1);
        triangle.push_back(p2);
        for(size_t idB=id+1; idB<vecP1.size(); idB++) {
            Point pp1=vecP1.at(idB);
            Point pp2=vecP2.at(idB);
            if(pp1!=p1 && pp2==p2 && existSegment(vecP1,vecP2,p1,pp1))
                triangle.push_back(pp1);
            else if(pp1==p1 && pp2!=p2 && existSegment(vecP1,vecP2,p2,pp2))
                triangle.push_back(pp2);
            else if(pp1!=p2 && pp2==p1 && existSegment(vecP1,vecP2,p2,pp1))
                triangle.push_back(pp1);
            else if(pp1==p2 && pp2!=p1 && existSegment(vecP1,vecP2,p1,pp2))
                triangle.push_back(pp2);
        }
        if(triangle.size()==3 && !existTriangle(vecTriangles,triangle.at(0),triangle.at(1),triangle.at(2)))
            vecTriangles.push_back(triangle);
    }
    return vecTriangles;
}

vector<vector<Point> > getTriangles(const vector<Point>& vecPoints, const vector<vector<Point> >& vecPoly)
{
    //Find unsharing segments
    vector<Point> vecSeg1, vecSeg2;
    for(size_t id=0; id<vecPoly.size(); id++) {
        for(auto it=vecPoly.at(id).begin(); it+1!=vecPoly.at(id).end();it++) {
            Point p1=(*it);
            Point p2=(*(it+1));
            if(!isBoundarySegment(vecPoints,p1,p2)) {
                if(!isShareSegment(vecPoly,p1,p2,id)) {
                    vecSeg1.push_back(p1);
                    vecSeg2.push_back(p2);
                }
            }
        }
        //Last segment
        Point p1=vecPoly.at(id).front();
        Point p2=vecPoly.at(id).back();
        if(!isBoundarySegment(vecPoints,p1,p2)) {
            if(!isShareSegment(vecPoly,p1,p2,id)) {
                vecSeg1.push_back(p1);
                vecSeg2.push_back(p2);
            }
        }
    }
    return getTriangles(vecSeg1,vecSeg2);
}
