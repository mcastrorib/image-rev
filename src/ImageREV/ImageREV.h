#ifndef IMAGEREV_H_
#define IMAGEREV_H_

// include Configuration file
#include "ImageREVHeader.h"

using namespace std;

class ImageREV
{
private:
    string method;
    bool validMethod;
    double *imageData;
    int imageWidth;
    int imageHeight;
    int imageDepth;
    int imageVolume;
    int maxREVSize;
    uchar poreColor;
    double *meanPorosities;
    double *stdPorosities; 

public:
    ImageREV()
    {
        this->method = "None";
        this->validMethod = false;
        this->imageData = NULL;
        this->imageWidth = 0;
        this->imageHeight = 0;
        this->imageDepth = 0;
        this->imageVolume = 0;
        this->maxREVSize = 0;
        this->poreColor = 0;
        this->meanPorosities = NULL;
        this->stdPorosities = NULL;
    }

    virtual ~ImageREV()
    {
        if(this->imageData != NULL)
        {
            delete this->imageData;
            this->imageData = NULL;
        }

        if(this->meanPorosities != NULL)
        {
            delete this->meanPorosities;
            this->meanPorosities = NULL;
        }

        if(this->stdPorosities != NULL)
        {
            delete this->stdPorosities;
            this->stdPorosities = NULL;
        }
    }

    void set(vector<cv::Mat> &_sourceImages, string _method, uchar _poreColor)
    {
        this->method = _method;
        this->poreColor = _poreColor;
        (*this).checkMethod();
        (*this).checkImage(_sourceImages);
        (*this).setImageDataArray();
        (*this).setPorosityArrays();
        (*this).convertImageData(_sourceImages);   
    }

    void checkMethod()
    {
        if (this->method == "complete" || this->method == "mc")
        {
            this->validMethod = true;
        } else
        {
            this->validMethod = false;
        }
    }

    void checkImage(vector<cv::Mat> &_sourceImages)
    {
        if(_sourceImages.size() > 0)
        {
            this->imageDepth = _sourceImages.size();
            this->imageHeight = _sourceImages[0].rows;
            this->imageWidth = _sourceImages[0].cols;
            this->imageVolume = this->imageWidth * this->imageHeight * this->imageDepth;
            (*this).setMaxREVSize(this->imageWidth, this->imageHeight, this->imageDepth);            
        }
    }

    void setMaxREVSize(int width, int height, int depth)
    {
        int min = width;
        if(height < min) min = height;
        if(depth < min) min = depth;
        this->maxREVSize = min;
    }

    void setImageDataArray()
    {
        this->imageData = new double[this->imageVolume];
    }

    void setPorosityArrays()
    {
        this->meanPorosities = new double[this->maxREVSize];
        this->stdPorosities = new double[this->maxREVSize];
    }

    void convertImageData(vector<cv::Mat> &_sourceImages)
    {
        uchar *currentPixel;
        int channels = _sourceImages[0].channels();
        int arrayPos;

        for (int z = 0; z < this->imageDepth; z++)
        {  
            for (int y = 0; y < this->imageHeight; y++)
            {
                currentPixel = _sourceImages[z].ptr<uchar>(y);
                for (int x = 0; x < this->imageWidth; x++)
                {
                    arrayPos = IDX2C_3D(x, y, z, this->imageWidth, this->imageHeight);
                    if(currentPixel[x*channels] == this->poreColor)
                    {
                        this->imageData[arrayPos] = 1.0;
                    } else {
                        this->imageData[arrayPos] = 0.0;
                    }
                }
            }
        }

        if(PRINT_DATA)
        {
            for (int z = 0; z < this->imageDepth; z++)
            {
                
                for (int y = 0; y < this->imageHeight; y++)
                {
                    for (int x = 0; x < this->imageWidth; x++)
                    {
                        int arrayPos = IDX2C_3D(x, y, z, this->imageWidth, this->imageHeight);
                        if(this->imageData[arrayPos] == 0.0)
                        {
                            cout << "% ";
                        } else cout << "^ ";
                    }

                    cout << endl;
                }
                cout << endl << endl;
            }
        }

        currentPixel = NULL;
    }

    void runAnalysis()
    {
        if(this->method == "complete")
        {
            (*this).runCompleteAnalysis();
        } else if(this->method == "mc")
        {
            (*this).runMonteCarloAnalysis();
        } else
        {
            cout << "REV method is not set" << endl;
        }
    }

    void runCompleteAnalysis()
    {
        cout << "REV complete method to be run:" << endl;
        double porositySum;
        double REVVolume;

        for (int size = 1; size <= this->maxREVSize; size++)
        {
            porositySum = 0.0;
            REVVolume = pow((double) size, 3.0);
            int localREVs = (this->imageWidth - size + 1) * (this->imageHeight - size + 1) * (this->imageDepth - size + 1);
            cout << "-- REVs of size " << size << " = " << localREVs << "\t REV volume: " << REVVolume << endl;
            
            for(int z = 0; z < (this->imageDepth - size + 1); z++)
            {
                for(int y = 0; y < (this->imageHeight - size + 1); y++)
                {
                    for(int x = 0; x < (this->imageWidth - size + 1); x++)
                    {
                        porositySum += (*this).computePorosity(x, y, z, size, REVVolume);
                    }                    
                }   
            }

            this->meanPorosities[size - 1] = porositySum / (double) localREVs;
            cout << "mean porosity: " << this->meanPorosities[size - 1] << endl;
        }
    }

    void runMonteCarloAnalysis()
    {
        cout << "REV Monte Carlo method to be run" << endl;
    }

    double computePorosity(int x0, int y0, int z0, int dim, double volume)
    {
        int xF = x0 + dim;
        int yF = y0 + dim;
        int zF = z0 + dim;
        int arrayPos;
        double poreVolume = 0.0;

        for(int z = z0; z < zF; z++)
        {
            for (int y = y0; y < yF; y++)
            {

                for (int x = x0; x < xF; x++)
                {
                    arrayPos = IDX2C_3D(x, y, z, this->imageWidth, this->imageHeight);
                    poreVolume += this->imageData[arrayPos];
                }
            }
        }

        return (poreVolume / volume);
    }
};

#endif