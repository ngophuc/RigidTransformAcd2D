#include "ConnectedComponent.h"

int **labelOfPixel;
int countRegion;

int findIdEquivalentRegion(vector<vector<int> > regions, int labelCurrent, int numberRegion)
{
    int rows = numberRegion;
    for (int i = 0; i <= rows; ++i)
    {
        int cols = regions.at(i).size();
        for (int j = 0; j < cols; ++j)
        {
            if (regions.at(i).at(j) == labelCurrent)
            {
                return i;
            }
        }
    }
    return -1;
}

void removeElementByValueFromVector(vector<int> &v, int value)
{
    for(vector<int>::iterator iter = v.begin(); iter != v.end(); ++iter)
    {
        if(*iter == value)
        {
            v.erase(iter);
        }
    }
}

void initValueVector(vector<int> &v, int value, int size)
{
    for(int i = 0; i < size; i++)
    {
        v.push_back(value);
    }
}

void mergeTwoVector(vector<int> &v1, vector<int> &v2)
{
    for (int i = 0; i < v2.size(); ++i)
    {
        v1.push_back(v2.at(i));
    }
    v2.clear();
}

void mergeTwoEquivalentRegion(vector<vector<int> > &equivalentRegions, int label1, int label2, int countRegion)
{
    int idEquivalentRegionCurrentPixel = findIdEquivalentRegion(equivalentRegions, label1, countRegion);
    int idEquivalentRegionNeighborPixel = findIdEquivalentRegion(equivalentRegions, label2, countRegion);
    if (idEquivalentRegionNeighborPixel == -1)
    {
        equivalentRegions.at(idEquivalentRegionCurrentPixel).push_back(label2);
    }
    else
    {
        if (idEquivalentRegionCurrentPixel > idEquivalentRegionNeighborPixel)
        {
            mergeTwoVector(equivalentRegions.at(idEquivalentRegionNeighborPixel), equivalentRegions.at(idEquivalentRegionCurrentPixel));
        }
        else if (idEquivalentRegionCurrentPixel < idEquivalentRegionNeighborPixel)
        {
            mergeTwoVector(equivalentRegions.at(idEquivalentRegionCurrentPixel), equivalentRegions.at(idEquivalentRegionNeighborPixel));
        }
    }
}

void initLabels(Domain domain)
{
    int widthImg = domain.upperBound()[0] - domain.lowerBound()[0];
    int heightImg = domain.upperBound()[1] - domain.lowerBound()[1];

    labelOfPixel = new int *[widthImg + 1];
    for (int i = domain.lowerBound()[0]; i <= domain.upperBound()[0]; ++i)
    {
        labelOfPixel[i] = new int[heightImg + 1];
        for (int j = domain.lowerBound()[1]; j <= domain.upperBound()[1]; ++j)
        {
            labelOfPixel[i][j] = 1000;
        }
    }
}

void coloringConnectedComponent(ConstImageAdapterForThresholderImage &thresholderImage, vector<int> &idRegions, vector<Region> &regions, const char* filename=NULL)
{
    Board2D aBoard;
    Domain domain = thresholderImage.domain();
    //if(filename!=NULL)
    //    aBoard << SetMode( expandedDomain.className(), "Paving" ) << domain;
    for (int i = 0; i < idRegions.size(); ++i)
    {
        Region r;
        Color colorOfPixel = Color( rand() % 192 + 64, rand() % 192 + 64, rand() % 192 + 64, 255);
        r.color = colorOfPixel;
        r.id = idRegions.at(i);

        for (int j = domain.lowerBound()[0]; j <= domain.upperBound()[0]; ++j)
        {
            for (int k = domain.lowerBound()[1]; k <= domain.upperBound()[1]; ++k)
            {
                Point p(j, k);
                if (labelOfPixel[j][k] == i)
                {
                    r.points.push_back(p);
                    if(filename!=NULL)
                        aBoard << CustomStyle( p.className(),
                                               new CustomPen( colorOfPixel, colorOfPixel, 1.0,
                                                              Board2D::Shape::SolidStyle,
                                                              Board2D::Shape::RoundCap,
                                                              Board2D::Shape::RoundJoin )) << p;
                }
            }
        }
        regions.push_back(r);
    }
    if(filename!=NULL)
        aBoard.saveSVG(filename);
}

vector<vector<int> > fourConnectedComponent(ConstImageAdapterForThresholderImage &thresholderImage, int value)
{
    int labelCurrent = 0;
    countRegion = 0;

    Domain domain = thresholderImage.domain();

    int heightImg = domain.upperBound()[1] - domain.lowerBound()[1];

    vector<vector<int> > equivalentRegions(heightImg, vector<int>( 1, -2 ));

    for (int i = domain.lowerBound()[0]; i <= domain.upperBound()[0]; ++i)
    {
        for (int j = domain.lowerBound()[1]; j <= domain.upperBound()[1]; ++j)
        {
            Point p0(i, j); int value0 = (int)thresholderImage(p0);

            if (value0 == value)
            {
                bool haveNeighbor = false;

                Point p2(i, j - 1);

                if(p2[0] >= domain.lowerBound()[0] && p2[0] <= domain.upperBound()[0] && p2[1] >= domain.lowerBound()[1] && p2[1] <= domain.upperBound()[1])
                {
                    int value2 = (int)thresholderImage(p2);
                    if (value0 == value2)
                    {
                        labelOfPixel[i][j] = labelOfPixel[i][j - 1];
                        haveNeighbor = true;
                    }
                }

                Point p3(i - 1, j);
                if(p3[0] >= domain.lowerBound()[0] && p3[0] <= domain.upperBound()[0] && p3[1] >= domain.lowerBound()[1] && p3[1] <= domain.upperBound()[1])
                {
                    int value3 = (int)thresholderImage(p3);
                    if (value0 == value3)
                    {
                        if (haveNeighbor)
                        {
                            if (labelOfPixel[i][j - 1] != labelOfPixel[i - 1][j])
                            {
                                if (labelOfPixel[i][j - 1] < labelOfPixel[i - 1][j])
                                {
                                    labelOfPixel[i][j] = labelOfPixel[i][j - 1];
                                    mergeTwoEquivalentRegion(equivalentRegions, labelOfPixel[i][j], labelOfPixel[i - 1][j], countRegion);
                                }
                                else
                                {
                                    labelOfPixel[i][j] = labelOfPixel[i - 1][j];
                                    mergeTwoEquivalentRegion(equivalentRegions, labelOfPixel[i][j], labelOfPixel[i][j - 1], countRegion);
                                }
                            }
                            else
                            {
                                labelOfPixel[i][j] = labelOfPixel[i][j - 1];
                            }
                        }
                        else
                        {
                            labelOfPixel[i][j] = labelOfPixel[i - 1][j];
                            haveNeighbor = true;
                        }
                    }
                }

                if (!haveNeighbor)
                {
                    labelOfPixel[i][j] = labelCurrent;
                    equivalentRegions.at(countRegion).at(0) = labelCurrent;

                    countRegion++;
                    labelCurrent++;
                }
            }
        }
    }
    return equivalentRegions;
}

vector<vector<int> > eightConnectedComponent(ConstImageAdapterForThresholderImage &thresholderImage, int value)
{
    Domain domain = thresholderImage.domain();

    vector<vector<int> > equivalentRegions = fourConnectedComponent(thresholderImage, value);

    for (int i = domain.lowerBound()[0]; i < domain.upperBound()[0]; ++i)
    {
        for (int j = domain.lowerBound()[1] + 1; j <= domain.upperBound()[1]; ++j)
        {
            Point p1(i, j); int value1 = (int)thresholderImage(p1);
            Point p2(i + 1, j); int value2 = (int)thresholderImage(p2);
            Point p3(i + 1, j - 1); int value3 = (int)thresholderImage(p3);
            Point p4(i, j - 1); int value4 = (int)thresholderImage(p4);
            /*
            | p1 | p2 |
            -----------
            | p4 | p3 |
            */
            if ( ( ( value1 == value ) && ( value2 != value ) && ( value3 == value ) && ( value4 != value ) ) )
            {
                mergeTwoEquivalentRegion(equivalentRegions, labelOfPixel[i][j], labelOfPixel[i + 1][j - 1], countRegion);
            }
            else if ( ( ( value1 != value ) && ( value2 == value ) && ( value3 != value ) && ( value4 == value ) ) )
            {
                mergeTwoEquivalentRegion(equivalentRegions, labelOfPixel[i][j - 1], labelOfPixel[i + 1][j], countRegion);
            }
        }
    }
    return equivalentRegions;
}

vector<int> mergeRegionPixelOneAndPixelZero(vector<vector<int> > &equivalentRegions, vector<vector<int> > &equivalentRegions1,
                                            ConstImageAdapterForThresholderImage &thresholderImage, int sizeEquivalentRegion, int sizeEquivalentRegion1, int numberPixelEnlarge=0)
{
    vector<int> regions;
    Domain domain = thresholderImage.domain();
    int count = 0;
    vector<vector<int> > labelOfPixelCopy(domain.upperBound()[0] - domain.lowerBound()[0] + 1, vector<int>( domain.upperBound()[1] - domain.lowerBound()[1] + 1, -1));
    for (int j = domain.lowerBound()[0]; j <= domain.upperBound()[0]; ++j)
    {
        for (int k = domain.lowerBound()[1]; k <= domain.upperBound()[1]; ++k)
        {
            labelOfPixelCopy[j + numberPixelEnlarge][k + numberPixelEnlarge] = labelOfPixel[j][k];
        }
    }

    for (int i = 0; i < sizeEquivalentRegion; ++i)
    {
        if (equivalentRegions.at(i).size() > 0)
        {
            regions.push_back(count);
            for (int j = domain.lowerBound()[0]; j <= domain.upperBound()[0]; ++j)
            {
                for (int k = domain.lowerBound()[1]; k <= domain.upperBound()[1]; ++k)
                {
                    Point p(j, k);
                    if ((int)thresholderImage(p) == 1)
                    {
                        if (findIdEquivalentRegion(equivalentRegions, labelOfPixel[j][k], countRegion) == i)
                        {
                            labelOfPixelCopy[j + numberPixelEnlarge][k + numberPixelEnlarge] = count;
                        }
                    }
                }
            }
            count++;
        }
    }

    for (int i = 0; i < sizeEquivalentRegion1; ++i)
    {
        if (equivalentRegions1.at(i).size() > 0)
        {
            regions.push_back(count);
            for (int j = domain.lowerBound()[0]; j <= domain.upperBound()[0]; ++j)
            {
                for (int k = domain.lowerBound()[1]; k <= domain.upperBound()[1]; ++k)
                {
                    Point p(j, k);
                    if ((int)thresholderImage(p) == 0)
                    {
                        if (findIdEquivalentRegion(equivalentRegions1, labelOfPixel[j][k], countRegion) == i)
                        {
                            labelOfPixelCopy[j + numberPixelEnlarge][k + numberPixelEnlarge] = count;
                        }
                    }
                }
            }
            count++;
        }
    }

    for (int j = domain.lowerBound()[0]; j <= domain.upperBound()[0]; ++j)
    {
        for (int k = domain.lowerBound()[1]; k <= domain.upperBound()[1]; ++k)
        {
            labelOfPixel[j][k] = labelOfPixelCopy[j + numberPixelEnlarge][k + numberPixelEnlarge];
        }
    }

    return regions;
}

void intersectionVectorV1ToVectorV2(vector<int> &v1, vector<int> & v2)
{
    for(int k1 = 0; k1 < v1.size(); k1++)
    {
        bool ok = false;
        for(int k2 = 0; k2 < v2.size(); k2++)
        {
            if (v1[k1] == v2[k2])
            {
                ok = true;
                break;
            }
        }

        if (!ok)
        {
            removeElementByValueFromVector(v1, v1[k1]);
        }
    }
}

void intersectionTwoVectors(vector<int> &v1, vector<int> &v2)
{
    intersectionVectorV1ToVectorV2(v1, v2);
    intersectionVectorV1ToVectorV2(v2, v1);
}

void generateTextImage(vector<Region> &regions, Domain domain, string filename, int numberPixelEnlarge=0)
{
    ofstream outfile;
    string output=filename+".txt";
    outfile.open(output.c_str());
    //outfile << domain.upperBound()[0] << " " << domain.upperBound()[1] << endl;
    //outfile << domain.lowerBound()[0] << " " << domain.lowerBound()[1] << endl;
    int width = domain.upperBound()[0] - domain.lowerBound()[0] + 1;
    int height = domain.upperBound()[1] - domain.lowerBound()[1] + 1;
    outfile<<height<<" "<<width<<endl;

    vector<vector<int> > idRegions( height, vector<int>(1, -1 ));
    for(int i = 0; i < height; i++)
    {
        for(int j = 1; j < width; j++)
        {
            idRegions.at(i).push_back(-1);
        }
    }

    for(int i = 0; i < regions.size(); i++)
    {
        vector<Point> points = regions.at(i).points;
        for (int j = 0; j < points.size(); ++j)
        {
            idRegions[points.at(j)[1] + numberPixelEnlarge][points.at(j)[0] + numberPixelEnlarge] = regions.at(i).id;
        }
    }

    for(int i = height-1; i >= 0; i--)//for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width - 1; j++)
        {
            outfile << idRegions[i][j] << " ";
        }
        outfile << idRegions[i][width - 1] << endl;
    }
    outfile.close();
}

int getConnectedComponent(string input, string output, int threshold, int typeConnectedObject, int typeConnectedBackground)
{
    functors::Identity df;
    //Read image
    Image image = DGtal::PGMReader<Image>::importPGM(input);
    Image::Domain domain = image.domain();
    //Threshold image
    DGtal::functors::Thresholder<Image::Value> t(threshold);
    ConstImageAdapterForThresholderImage thresholderImage(image, domain, df, t);
    Domain domainThresholderImage = thresholderImage.domain();
    //Init labels
    initLabels(domainThresholderImage);
    //Compute connected component of objects
    vector<vector<int> > equivalentRegions;
    if (typeConnectedObject == 4)//typeConnectedPixelOne
        equivalentRegions = fourConnectedComponent(thresholderImage, 1);
    else
        equivalentRegions = eightConnectedComponent(thresholderImage, 1);
    int sizeEquivalentRegion = countRegion;
    //Compute connected component of background
    vector<vector<int> > equivalentRegions1;
    if (typeConnectedBackground == 4)//typeConnectedPixelZero
        equivalentRegions1 = fourConnectedComponent(thresholderImage, 0);
    else
        equivalentRegions1 = eightConnectedComponent(thresholderImage, 0);
    int sizeEquivalentRegion1 = countRegion;
    //Merger equivalent regions
    vector<int> idRegions = mergeRegionPixelOneAndPixelZero(equivalentRegions, equivalentRegions1, thresholderImage, sizeEquivalentRegion, sizeEquivalentRegion1);
    //Gengerate and draw regions
    vector<Region> regions;
    //string filename=output+".svg";
    coloringConnectedComponent(thresholderImage, idRegions, regions);//,filename.c_str()
    generateTextImage(regions, domainThresholderImage, output);

    Point p(0,0);//(regions.at(0).points.at(0)[0], regions.at(0).points.at(0)[1]);
    return (int)thresholderImage(p);
}
