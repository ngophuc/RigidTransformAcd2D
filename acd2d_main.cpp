// general header files
#include <string>
#include <fstream>
#include <time.h>
#include <vector>

#include "src/acd2d.h"
#include "src/acd2d_concavity.h"
#include "src/acd2d_data.h"

using namespace std;
using namespace acd2d;

void load(cd_2d& cd, string filename);
void save(cd_2d& cd, string filename);
void decomposeAll(cd_2d& cd, string concavity_measure, double alpha, double beta, double tau);
cd_polygon readPolygon(string filename);
void writePolygon(ofstream &os, const cd_poly &poly);
void writeDecomposition(string filename, string output);

int main(int argc, char ** argv)
{
    cd_2d cd;
    //set some initial values...
    string g_filename; // filename
    string g_concavity_measure="hybrid2"; // concavity_measure name
    double g_alpha=0.0, g_beta=0.0; // for selecting cutting direction
    double g_tau=0; //tolerance
    bool   g_saveDecomposition=false;

    //parse the argument
    for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){
            switch(argv[i][1]){
            case 'm': g_concavity_measure=argv[++i]; break;
            case 's': g_saveDecomposition=true; break;
            default:
                break; //simply ignore....
            }
        }
        else{
            g_filename=argv[i];
        }
    }

    load(cd,g_filename);
    if(cd.getTodoList().empty()) return 1;
    decomposeAll(cd,g_concavity_measure,g_alpha,g_beta,g_tau);
    if(g_saveDecomposition) {
        string simpleName = g_filename.substr(0,g_filename.find_last_of("."));
        string filename=simpleName+"-acd.poly";
        save(cd,filename);
        string output=simpleName;//+"-acd";
        writeDecomposition(filename,output);
    }
    cd.destroy();

    return 0;
}

void createPolys(const string& filename, cd_2d& cd)
{
    //read polygon
    cd_polygon poly;
    ifstream fin(filename.c_str());
    if( fin.good()==false ){
        cerr<<"! ERROR: can NOT open file : "<<filename<<endl;
        return;
    }
    fin>>poly;
    fin.close();

    ///poly.normalize();
    cd.addPolygon(poly);

    //there is nothing todo...
    if(cd.getTodoList().empty()) return;

    Point2d O;    //center
    double box[4]={0,0,0,0};
    const cd_poly& poly1=*cd.getTodoList().begin()->begin();
    polyBox(poly1,box);
    O=polyCenter(box);
    double R= sqr(O[0]-box[0])+sqr(O[1]-box[2]);
    R=sqrt((float)R);

    //rebuild box
    box[0]=O[0]-R;
    box[1]=O[0]+R;
    box[2]=O[1]-R;
    box[3]=O[1]+R;
}


void load(cd_2d& cd, string filename)
{
    createPolys(filename,cd);
}

void save(cd_2d& cd, string filename)
{
    save_polys(filename,cd);
    cout<<"- Save to file: "<<filename<<endl;
}

void decomposeAll(cd_2d& cd, string concavity_measure, double alpha, double beta, double tau)
{
    IConcavityMeasure * measure= ConcavityMeasureFac::createMeasure(concavity_measure);
    cd.updateCutDirParameters(alpha,beta);

    if(concavity_measure=="hybrid2")
        ((HybridMeasurement2*)measure)->setTau(tau);

    clock_t start=clock();
    cd.decomposeAll(tau,measure);
    int time=clock()-start;
    cout<<"- Decompose All Takes "<<((double)(time))/CLOCKS_PER_SEC<<" secs"<<endl;
    delete measure;
}

void writeDecomposition(string filename, string output)
{
    //Read poly file
    cd_polygon poly;
    ifstream fin(filename.c_str());
    if( fin.good()==false ){
        cerr<<"! ERROR: can NOT open file : "<<filename<<endl;
        return;
    }
    fin>>poly;
    fin.close();
    //Convert polygon to point coordinates
    vector<vector<int> > polyline;
    vector<vector<int> > coordX;
    vector<vector<int> > coordY;
    for(list<cd_poly>::const_iterator it=poly.begin();it!=poly.end();it++) {
        cd_poly p=*it;
        vector<int> pl,x,y;
        ///cout<<p.getSize()<<" "<<((p.getType()==cd_poly::PIN)?"in":"out")<<"\n";
        cd_vertex * ptr=p.getHead();
        do{
            ///cout<<ptr->getPos()[0]<<" "<<ptr->getPos()[1]<<"\n";
            x.push_back(ptr->getPos()[0]);
            y.push_back(ptr->getPos()[1]);
            ptr=ptr->getNext();
        }while(ptr!=p.getHead());

        for(int i=0;i<p.getSize();i++)
            //cout<<i+1<<" ";
            pl.push_back(i);
        //cout<<"\n";
        polyline.push_back(pl);
        coordX.push_back(x);
        coordY.push_back(y);
    }

    //Write poly file
    ofstream outFile;
    string outFilename;
    //Write nb of convex polylines of the decomposition
    outFilename=output+".txt";
    outFile.open(outFilename.c_str());
    if (!outFile.is_open()) {
        cerr<<"! ERROR: can NOT open file : "<<filename<<endl;
        return;
    }
    outFile<<polyline.size()<<endl;
    outFile.close();
    //Write convex polylines
    for(size_t it_p=0; it_p<polyline.size(); it_p++) {
        outFilename=output+"_"+to_string(it_p)+".sdp";
        outFile.open(outFilename.c_str());
        if (!outFile.is_open()) {
            cerr<<"! ERROR: can NOT open file : "<<filename<<endl;
            return;
        }
        for(size_t it=0; it<polyline.at(it_p).size(); it++)
            outFile<<coordX.at(it_p).at(polyline.at(it_p).at(it))<<" "<<coordY.at(it_p).at(polyline.at(it_p).at(it))<<endl;
        outFile.close();
    }
}
