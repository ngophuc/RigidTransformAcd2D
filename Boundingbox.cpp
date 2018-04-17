#include "Boundingbox.h"

BoundingBox::BoundingBox()
{
    topLeft=PointD(0,0);
    bottomLeft=PointD(0,0);
    bottomRight=PointD(0,0);
    topRight=PointD(0,0);
}

BoundingBox::BoundingBox(PointD p1, PointD p2, PointD p3, PointD p4)
{
    topLeft=p1;
    bottomLeft=p2;
    bottomRight=p3;
    topRight=p4;
}

void BoundingBox::getBoundingBox(PointD& p1, PointD& p2, PointD& p3, PointD& p4) const
{
    p1=topLeft;
    p2=bottomLeft;
    p3=bottomRight;
    p4=topRight;
}

BoundingBox BoundingBox::getBoundingBox() const
{
    return BoundingBox(topLeft,bottomLeft,bottomRight,topRight);
}

Point BoundingBox::getSize()
{
    return Point(abs(topRight[0]-bottomLeft[0])+1,abs(topRight[1]-bottomLeft[1])+1);
}

void BoundingBox::setBoundingBox(const PointD p1, const PointD p2, const PointD p3, const PointD p4)
{
    topLeft=p1;
    bottomLeft=p2;
    bottomRight=p3;
    topRight=p4;
}

void BoundingBox::draw(Board2D& aBoard) const
{
    aBoard.drawLine(topLeft[0],topLeft[1],bottomLeft[0],bottomLeft[1]);
    aBoard.drawLine(topLeft[0],topLeft[1],topRight[0],topRight[1]);
    aBoard.drawLine(bottomRight[0],bottomRight[1],bottomLeft[0],bottomLeft[1]);
    aBoard.drawLine(bottomRight[0],bottomRight[1],topRight[0],topRight[1]);
}

BoundingBox BoundingBox::transformBoundingBox(const transformation T)
{
    PointD tp1=transformPoint(topLeft,T);
    PointD tp2=transformPoint(bottomLeft,T);
    PointD tp3=transformPoint(bottomRight,T);
    PointD tp4=transformPoint(topRight,T);
    return BoundingBox(tp1,tp2,tp3,tp4);
}

BoundingBox BoundingBox::transformBoundingBox(const transformation T, int a, int b, int mu)
{
    ///cout<<"Transformation: "<<T[0]<<","<<T[1]<<","<<T[2]<<endl;
    ///cout<<"a="<<a<<", b="<<b<<", mu="<<mu<<endl;
    ///cout<<"BB before transformation :"<<topLeft<<","<<bottomLeft<<","<<bottomRight<<","<<topRight<<endl;
    //DSS: mu<= ax-by <= mu+omega-1 with omega=|a|+|b|
    //Central line of bounding box: ax-by=mu+(omega-1)/2
    double aC=(double)a;
    double bC=(double)b;
    double cC=(double)mu+(fabs(aC)+fabs(bC)-1.0)/2.0;
    PointD pLC=projectPoint(aC,bC,cC,topLeft);
    PointD pRC=projectPoint(aC,bC,cC,topRight);
    PointD tpLC=transformPoint(pLC,T);//cout<<"(01):"<<tpLC<<"=?"<<pLC<<endl;
    PointD tpRC=transformPoint(pRC,T);//cout<<"(02):"<<tpRC<<"=?"<<pRC<<endl;

    double taC=tpLC[1]-tpRC[1];
    double tbC=tpLC[0]-tpRC[0];
    double tcC=taC*tpLC[0]-tbC*tpLC[1];//mu+(omega-1)/2
    double tOmega;
#if defined(EPSILON)
    tOmega=(fabs(taC)+fabs(tbC)-1.0)/2.0+EPSILON;
#else
    tOmega=(fabs(taC)+fabs(tbC)-1.0)/2.0;
#endif
    PointD tp1=projectPoint(taC,tbC,tcC-tOmega,tpLC);//topLeft//mu
    PointD tp2=projectPoint(taC,tbC,tcC+tOmega,tpLC);//bottomLeft//omega
    PointD tp3=projectPoint(taC,tbC,tcC+tOmega,tpRC);//bottomRight//omega
    PointD tp4=projectPoint(taC,tbC,tcC-tOmega,tpRC);//topRight//mu
    ///cout<<"BB after transformation :"<<tp1<<","<<tp2<<","<<tp3<<","<<tp4<<endl;
    return BoundingBox(tp1,tp2,tp3,tp4);
}

BoundingBox BoundingBox::transformBoundingBox(const transformation T, int a, int b, int mu, int imgSize)
{
    //DSS: mu<= ax-by <= mu+omega-1 with omega=|a|+|b|
    //Central line of bounding box: ax-by=mu+(omega-1)/2
    double aC=(double)a;
    double bC=(double)b;
    double cC=(double)mu+(fabs(aC)+fabs(bC)-1.0)/2.0;
    PointD pLC=projectPoint(aC,bC,cC,topLeft);
    PointD pRC=projectPoint(aC,bC,cC,topRight);
    PointD tpLC=transformPoint(pLC,T,imgSize);//cout<<"(01):"<<tpLC<<"=?"<<pLC<<endl;
    PointD tpRC=transformPoint(pRC,T,imgSize);//cout<<"(02):"<<tpRC<<"=?"<<pRC<<endl;

    double taC=tpLC[1]-tpRC[1];
    double tbC=tpLC[0]-tpRC[0];
    double tcC=taC*tpLC[0]-tbC*tpLC[1];//mu+(omega-1)/2
    double tOmega;
#if defined(EPSILON)
    tOmega=(fabs(taC)+fabs(tbC)-1.0)/2.0+EPSILON;
#else
    tOmega=(fabs(taC)+fabs(tbC)-1.0)/2.0;
#endif
    PointD tp1=projectPoint(taC,tbC,tcC-tOmega,tpLC);//topLeft//mu
    PointD tp2=projectPoint(taC,tbC,tcC+tOmega,tpLC);//bottomLeft//omega
    PointD tp3=projectPoint(taC,tbC,tcC+tOmega,tpRC);//bottomRight//omega
    PointD tp4=projectPoint(taC,tbC,tcC-tOmega,tpRC);//topRight//mu
    return BoundingBox(tp1,tp2,tp3,tp4);
}

bool BoundingBox::isInside(const Point &p) const
{
    //(0<AM⋅AB<AB⋅AB)∧(0<AM⋅AD<AD⋅AD)
    PointD M=PointD(p[0],p[1]);
    PointD A=topLeft;
    PointD B=topRight;
    PointD D=bottomLeft;

    double v1=scalarProduct(vectorValue(A,M),vectorValue(A,B));
    double v2=scalarProduct(vectorValue(A,B),vectorValue(A,B));
    double v3=scalarProduct(vectorValue(A,M),vectorValue(A,D));
    double v4=scalarProduct(vectorValue(A,D),vectorValue(A,D));
    /*
    if(p==Point(2,8) || p==Point(8,7))
        //cout<<p<<": "<<v1*v2<<">=0 && "<<v1*v2<<"<="<<v2*v2<<" and "<<v3*v4<<">=0 && "<<v3*v4<<"<="<<v4*v4<<endl;
        cout<<p<<": "<<(v1*v2>=0)<<"&&"<<(v1*v2<=v2*v2)<<" and "<<(v3*v4>=0)<<"&&"<<(v3*v4<=v4*v4)<<endl;
    */
#if defined(EPSILON)
    return (((v1*v2)>=0 && (v1*v2-v2*v2)<=EPSILON)&&((v3*v4)>=0 && (v3*v4-v4*v4)<=EPSILON));
#else
    return ((v1*v2>=0 && v1*v2<=v2*v2)&&(v3*v4>=0 && v3*v4<=v4*v4));
#endif
}

vector<Point> BoundingBox::getInsidePoints() const
{
    vector<Point> points;
    double minX=std::min(topLeft[0],std::min(topRight[0],std::min(bottomLeft[0],bottomRight[0])));
    double maxX=std::max(topLeft[0],std::max(topRight[0],std::max(bottomLeft[0],bottomRight[0])));
    double minY=std::min(topLeft[1],std::min(topRight[1],std::min(bottomLeft[1],bottomRight[1])));
    double maxY=std::max(topLeft[1],std::max(topRight[1],std::max(bottomLeft[1],bottomRight[1])));
    //FIXME: To optimize !!!
    for(int x=(int)minX;x<=(int)maxX;x++)//for(int x=(int)minX-1;x<=(int)maxX+1;x++)
        for(int y=(int)minY;y<=(int)maxY;y++)//for(int y=(int)minY-1;y<=(int)maxY+1;y++)
        {
            Point p(x,y);
            if(isInside(p))
                points.push_back(p);
        }
    return points;
}

BoundingBox findBoundingBox(const vector<Point>& vec)
{
    int minX=vec.at(0)[0], minY=vec.at(0)[1];
    int maxX=vec.at(0)[0], maxY=vec.at(0)[1];
    for(vector<Point>::const_iterator it=vec.begin(); it!=vec.end(); it++)
    {
        if((*it)[0]<minX)
            minX=(*it)[0];
        if((*it)[1]<minY)
            minY=(*it)[1];
        if((*it)[0]>maxX)
            maxX=(*it)[0];
        if((*it)[1]>maxY)
            maxY=(*it)[1];
    }
    //cout<<"min=("<<minX<<","<<minY<<") and max=("<<maxX<<","<<maxY<<")"<<endl;
    return BoundingBox(Point(minX,maxY),Point(minX,minY),Point(maxX,minY),Point(maxX,maxY));
}

BoundingBox findBoundingBox(const vector<RealPoint>& vec)
{
    double minX=vec.at(0)[0], minY=vec.at(0)[1];
    double maxX=vec.at(0)[0], maxY=vec.at(0)[1];
    for(vector<RealPoint>::const_iterator it=vec.begin(); it!=vec.end(); it++)
    {
        if((*it)[0]<minX)
            minX=(*it)[0];
        if((*it)[1]<minY)
            minY=(*it)[1];
        if((*it)[0]>maxX)
            maxX=(*it)[0];
        if((*it)[1]>maxY)
            maxY=(*it)[1];
    }
    //cout<<"min=("<<minX<<","<<minY<<") and max=("<<maxX<<","<<maxY<<")"<<endl;
    return BoundingBox(RealPoint(minX,maxY),RealPoint(minX,minY),RealPoint(maxX,minY),RealPoint(maxX,maxY));
}
