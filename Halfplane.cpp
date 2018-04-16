#include "Halfplane.h"

HalfPlane::HalfPlane(int a, int b, int c, int s)
{
    this->a=a;
    this->b=b;
    this->c=c;
    this->sign=s;
    this->p1=Point(0,0);
    this->p2=Point(0,0);
}

HalfPlane::HalfPlane(int a, int b, int c, int s, Point p1, Point p2)
{
    this->a=a;
    this->b=b;
    this->c=c;
    this->sign=s;
    this->p1=p1;
    this->p2=p2;
}

bool HalfPlane::isBelong(const Point& p) const
{
    return (sign*(a*p[0]+b*p[1]+c)) >= 0;
}

vector<Point> HalfPlane::getPoints(const Point bg, const Point hd)
{
    vector<Point> vecP;
    for(int i=bg[0]; i<=hd[0]; i++)
        for(int j=bg[1]; j<=hd[1]; j++)
        {
            Point p(i,j);
            if(isBelong(p))
                vecP.push_back(p);
        }
    return vecP;
}

HalfPlane HalfPlane::transformHalfplane(const transformation t) const
{
    //Conversion of transformation
    int txa,txb,tya,tyb;
    convertDecimal2Fraction(t[0],txa,txb);
    convertDecimal2Fraction(t[1],tya,tyb);
    int ca,cb,cc;
    convertAngle2Pythagore(t[2],ca,cb,cc);
    ///cout<<"Translation x "<<t[0]<<" equivalent "<<txa<<"/"<<txb<<endl;
    ///cout<<"Translation y "<<t[1]<<" equivalent "<<tya<<"/"<<tyb<<endl;
    ///cout<<"Rotation "<<t[2]<<" equivalent "<<ca<<","<<cb<<","<<cc<<endl;
    /**** Discrete version *******
    if(ca==0 && cb==0 && cc==0)
        return HalfPlane(a,b,c,sign);
    int ra=txb*tyb*(a*ca-b*cb);
    int rb=txb*tyb*(a*cb+b*ca);
    int rc=c*cc*txb*tyb+txa*tyb*(a*ca-b*cb)+txb*tya*(a*cb+b*ca);
    int p=abs(pgcd(ra,rb));
    if(p!=1)
    {
        ra=ra/p;
        rb=rb/p;
        rc=rc/p;
    }
    return HalfPlane(ra,rb,rc,sign);
    /**** Discrete version *******/
    //Translation
    int ta=a;
    int tb=b;
    int tc=c+ceil(a*(double)txa/txb-b*(double)tya/tyb);
    //Rotation
    if(ca==0 && cb==0 && cc==0)
        return HalfPlane(ta,tb,tc,sign);
    //FIXME angle:kPi/2 => symetry
    int ra=tb*ca+ta*cb;
    int rb=tb*cb-ta*ca;
    int rc=cc*tc;//romega=abs(b*ca+a*cb)+abs(b*cb-a*ca);
    int p=abs(pgcd(ra,rb));
    if(p!=1)
    {
        ra=ra/p;
        rb=rb/p;
        rc=rc/p;
    }
    return HalfPlane(ra,rb,rc,sign);
    /**** Discrete version *******/

    /**** Approximation version ****
    double r11=cos(t[2]);
    double r12=-sin(t[2]);
    double r21=sin(t[2]);
    double r22=cos(t[2]);
    Point rp1=Point(round(p1[0]*r11+p1[1]*r12+t[0]),round(p1[0]*r21+p1[1]*r22+t[1]));
    Point rp2=Point(round(p2[0]*r11+p2[1]*r12+t[0]),round(p2[0]*r21+p2[1]*r22+t[1]));

    //(y1 – y2)x + (x2 – x1)y + (x1y2 – x2y1) = 0
    int ra=rp1[1]-rp2[1];
    int rb=rp2[0]-rp1[0];
    int rc=rp1[0]*rp2[1]-rp2[0]*rp1[1];
    int rPgcd=abs(pgcd(ra,rb));
    if(rPgcd!=1)
    {
        ra=ra/rPgcd;
        rb=rb/rPgcd;
        rc=rc/rPgcd;
    }
    //cout<<"rp1="<<rp1<<" rp2="<<rp2<<endl;
    //cout<<"("<<ra<<","<<rb<<","<<rc<<","<<","<<sign<<")"<<endl<<endl;
    return (HalfPlane(ra,rb,rc,sign,rp1,rp2));
    /**** Approximation version ****/
}

HalfPlane HalfPlane::transformPoint(const transformation t) const
{
    double alpha=t[2];
    Point rp1=Point(int(cos(alpha)*p1[0]-sin(alpha)*p1[1]+t[0]),int(sin(alpha)*p1[0]+cos(alpha)*p1[1]+t[1]));
    Point rp2=Point(int(cos(alpha)*p2[0]-sin(alpha)*p2[1]+t[0]),int(sin(alpha)*p2[0]+cos(alpha)*p2[1]+t[1]));
    return (HalfPlane(0,0,0,0,rp1,rp2));
}

double HalfPlane::distancePoint(const Point p)
{
    return ((double(a*p[0]+b*p[1]+c))/sqrt(a*a+b*b));
}

bool isBelong(const vector<HalfPlane>& vecHP, const Point& p)
{
    for(size_t it=0; it<vecHP.size(); it++)
        if(!vecHP.at(it).isBelong(p))
            return false;
    return true;
}
