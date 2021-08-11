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
    int *imageData;
    int imageWidth;
    int imageHeight;
    int imageDepth;
    int imageVolume;
    bool validVolume;
    int maxREVSize;
    uchar poreColor;
    int REVsizes;
    int maxREVsPerSize;
    int *REVsizesData;
    double *REVporositiesData;
     

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
        this->validVolume = false;
        this->maxREVSize = 0;
        this->poreColor = 0;
        this->REVsizes = 0;
        this->maxREVsPerSize = 0;
        this->REVsizesData = NULL;
        this->REVporositiesData = NULL;
    }

    virtual ~ImageREV()
    {
        if(this->imageData != NULL)
        {
            delete [] this->imageData;
            this->imageData = NULL;
        }

        if(this->REVporositiesData != NULL)
        {
            delete [] this->REVporositiesData;
            this->REVporositiesData = NULL;
        }

        if(this->REVsizesData != NULL)
        {
            delete [] this->REVsizesData;
            this->REVsizesData = NULL;
        }
    }

    string getMethod(){ return this->method; }
    bool getValidMethod() { return this->validMethod; };
    int* getImageData() { return this->imageData; };
    int getImageWidth(){ return this->imageWidth; };
    int getImageHeight(){ return this->imageHeight;};
    int getImageDepth(){ return this->imageDepth; };
    int getImageVolume(){ return this->imageVolume; };
    bool getValidVolume(){ return this->validVolume; };
    int getMaxREVSize(){ return this->maxREVSize; };
    uchar getPoreColor(){ return this->poreColor;};
    int getREVsizes(){ return this->REVsizes; };
    int getMaxREVsPerSize(){ return this->maxREVsPerSize; };
    int* getREVsizesData(){ return this->REVsizesData; };
    double* getREVporositiesData(){ return this->REVporositiesData; };
    int getREVsizesData(int idx)
    { 
        if(idx < this->REVsizes) return this->REVsizesData[idx];
        else return -1; 
    }

    double getREVporositiesData(int idx)
    { 
        if(idx < this->REVsizes) return this->REVporositiesData[idx]; 
        else return -1.0;
    }    

    void set(vector<cv::Mat> &_sourceImages, string _method, uchar _poreColor, int _REVsizes, int _REVsamples)
    {
        this->method = _method;
        this->poreColor = _poreColor;
        this->maxREVsPerSize = _REVsamples;
        (*this).checkMethod();
        (*this).checkImage(_sourceImages);
        (*this).setREVSizes(_REVsizes);
        (*this).setImageDataArray();
        (*this).setPorosityArray();
        (*this).setSizesArray();
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

        double vol = ((double) this->imageWidth) * ((double) this->imageHeight) * ((double) this->imageDepth);
        double max_vol = (double) INT_MAX;
        if(vol < max_vol)
        {
            this->validVolume = true;
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
        this->imageData = new int[this->imageVolume];
    }

    void setREVSizes(int _maxSizes)
    {   
        this->REVsizes = _maxSizes;
        if(this->maxREVSize < this->REVsizes)
        {
            this->REVsizes = this->maxREVSize;
        }
    }

    void setPorosityArray()
    {
        // Alloc memory for 'REVporosities' array
        this->REVporositiesData = new double[this->REVsizes];
    }

    void setSizesArray()
    {
        // Alloc memory for 'REVsizes' array
        this->REVsizesData = new int[this->REVsizes];

        double minSize = this->maxREVSize / (double) this->REVsizes;
        int maxSize = this->maxREVSize;
        
        for(int sIdx = 0; sIdx < this->REVsizes; sIdx++)
        {
            this->REVsizesData[sIdx] = round(minSize + ((double) sIdx) * minSize);
        }
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
                        this->imageData[arrayPos] = 1;
                    } else {
                        this->imageData[arrayPos] = 0;
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
                        if(this->imageData[arrayPos] == 0)
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
        cout << "REV complete method to be run: (to be developed)" << endl;
        double porositySum;
        double REVVolume;
        return;

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

            this->REVporositiesData[size - 1] = porositySum / (double) localREVs;
            cout << "mean porosity: " << this->REVporositiesData[size - 1] << endl;
        }
    }

    void runMonteCarloAnalysis()
    {
        cout << "REV Monte Carlo method to be run" << endl;
        double porositySum;
        double REVVolume;
        int Xmax, Ymax, Zmax;
        int size;

        for (int sIdx = 0; sIdx < this->REVsizes; sIdx++)
        {
            porositySum = 0.0;
            size = this->REVsizesData[sIdx];
            REVVolume = pow((double) size, 3.0);
            int localREVs = (this->imageWidth - size + 1) * (this->imageHeight - size + 1) * (this->imageDepth - size + 1);
            if(localREVs > this->maxREVsPerSize) 
            { 
                   localREVs = maxREVsPerSize;
            }

            cout << "- " << sIdx+1 << "/" << this->REVsizes << " -";
            cout << "REVs of size " << size << " (" << this->REVsizesData[sIdx] << ")";
            cout << " = " << localREVs << "\t REV volume: " << REVVolume << endl;
            
            Xmax = (this->imageWidth - size);
            Ymax = (this->imageHeight - size);
            Zmax = (this->imageDepth - size);
            // cout << "-- Max values: " << Xmax << "\t " << Ymax << "\t " << Zmax << endl;

            mRNG rngX(0, Xmax);
            mRNG rngY(0, Ymax);
            mRNG rngZ(0, Zmax);            

            for(int revIdx = 0; revIdx < localREVs; revIdx++)
            { 
                // cout << "-- " << rngX() << "\t " << rngY() << "\t " << rngZ() << endl;
                porositySum += (*this).computePorosity(rngX(), rngY(), rngZ(), size, REVVolume);        
            }

            this->REVporositiesData[sIdx] = porositySum / (double) localREVs;
            cout << "mean porosity: " << this->REVporositiesData[sIdx] << endl;
        }
    }

    double computePorosity(int x0, int y0, int z0, int dim, double volume)
    {
        int xF = x0 + dim;
        int yF = y0 + dim;
        int zF = z0 + dim;
        int arrayPos;
        int poreVolume = 0;

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

        return ((double) poreVolume / volume);
    }
};

#endif