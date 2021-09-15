#ifndef IMAGEREVANALYZER_H_
#define IMAGEREVANALYZER_H_

// include Configuration file
#include "ImageREVHeader.h"

using namespace std;

class ImageREVAnalyzer
{
private:
    string method;
    bool validMethod;
    uchar *imageData;
    int imageWidth;
    int imageHeight;
    int imageDepth;
    unsigned long int imageVolume;
    bool validVolume;
    int maxREVSize;
    uchar poreColor;
    int REVsizes;
    int maxREVsPerSize;
    int *REVsizesData;
    double *REVporositiesData;
    bool validAnalysis;
     

public:
    ImageREVAnalyzer()
    {
        this->method = "None";
        this->validMethod = false;
        this->validAnalysis = false;
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

    virtual ~ImageREVAnalyzer()
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
    bool getValidAnalysis() { return this->validAnalysis; };
    uchar* getImageData() { return this->imageData; };
    int getImageWidth(){ return this->imageWidth; };
    int getImageHeight(){ return this->imageHeight;};
    int getImageDepth(){ return this->imageDepth; };
    unsigned long int getImageVolume(){ return this->imageVolume; };
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

    void updateValidAnalysis(bool _valid)
    { 
        this->validAnalysis = _valid;
    }

    void set_old(vector<cv::Mat> &_sourceImages, string _method, uchar _poreColor, int _REVsizes, int _REVsamples)
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

    void set(int _count, string _method, uchar _poreColor, int _REVsamples)
    {
        this->imageDepth = _count;
        this->method = _method;
        this->poreColor = _poreColor;
        this->maxREVsPerSize = _REVsamples;
        (*this).checkMethod();
        (*this).updateValidAnalysis(false);
    }

    void checkFirstImage(cv::Mat _sourceImage, int _REVsizes)
    {
        this->imageHeight = _sourceImage.rows;
        this->imageWidth = _sourceImage.cols;
        this->imageVolume = ((unsigned long int) this->imageWidth) * ((unsigned long int) this->imageHeight) *((unsigned long int) this->imageDepth);
        (*this).setMaxREVSize(this->imageWidth, this->imageHeight, this->imageDepth);  
        (*this).setREVSizes(_REVsizes);          
       
        double vol = ((double) this->imageWidth) * ((double) this->imageHeight) * ((double) this->imageDepth);
        double max_vol = (double) INT_MAX;
        if(vol < max_vol)
        {
            this->validVolume = true;
        } 
    }

    void readImage(cv::Mat _image, int _slice)
    {
        uchar *currentPixel;
        int channels = _image.channels();
        unsigned long int arrayPos;
        unsigned long int slice = _slice;
          
        for (unsigned long int y = 0; y < this->imageHeight; y++)
        {
            currentPixel = _image.ptr<uchar>(y);
            for (unsigned long int x = 0; x < this->imageWidth; x++)
            {
                arrayPos = IDX2C_3D(x, y, slice, (unsigned long int) this->imageWidth, (unsigned long int) this->imageHeight);
                if(currentPixel[x*channels] == this->poreColor)
                {
                    this->imageData[arrayPos] = 1;
                } else {
                    this->imageData[arrayPos] = 0;
                }
            }
        }
    
        if(PRINT_DATA)
        {
                       
            for (int y = 0; y < this->imageHeight; y++)
            {
                for (int x = 0; x < this->imageWidth; x++)
                {
                    arrayPos = IDX2C_3D(x, y, _slice, (unsigned long int) this->imageWidth, (unsigned long int) this->imageHeight);
                    if(this->imageData[arrayPos] == 0)
                    {
                        cout << "% ";
                    } else cout << "^ ";
                }

                cout << endl;
            }
            cout << endl << endl;        
        }

        currentPixel = NULL;
    }

    void setArrays()
    {
        (*this).setImageDataArray();
        (*this).setPorosityArray();
        (*this).setSizesArray();
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
        cout << "image props: ";
        cout << "(" <<  this->imageWidth;
        cout << ", " <<  this->imageHeight;
        cout << ", " <<  this->imageDepth;
        cout << ") vol: " << this->imageVolume << endl; 
        cout << "mem to alloc: " << (sizeof(uchar) * this->imageVolume) / (pow(10.0, 9.0)) << " GBytes." << endl;
        this->imageData = new uchar[this->imageVolume];
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
        unsigned long int arrayPos;

        for (unsigned long int z = 0; z < this->imageDepth; z++)
        {  
            for (unsigned long int y = 0; y < this->imageHeight; y++)
            {
                currentPixel = _sourceImages[z].ptr<uchar>(y);
                for (unsigned long int x = 0; x < this->imageWidth; x++)
                {
                    arrayPos = IDX2C_3D(x, y, z, (unsigned long int) this->imageWidth, (unsigned long int) this->imageHeight);
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
                        arrayPos = IDX2C_3D(x, y, z, (unsigned long int) this->imageWidth, (unsigned long int) this->imageHeight);
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
            (*this).updateValidAnalysis(true);
        } 
        else if(this->method == "mc")
        {
            (*this).runMonteCarloAnalysis();
            (*this).updateValidAnalysis(true);
        } 
        else if(this->method == "center")
        {
            (*this).runCentralAnalysis();
            (*this).updateValidAnalysis(true);
        }
        else
        {
            cout << "REV method is not set" << endl;
        }
    }

    void runCompleteAnalysis()
    {
        cout << "- REV analysis by complete method: (to be developed)" << endl;
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
            cout << ", mean porosity: " << this->REVporositiesData[size - 1] << endl;
        }
    }

    void runMonteCarloAnalysis()
    {
        cout << "- Running REV analysis by Monte Carlo method" << endl;
        double porositySum;
        double REVVolume;
        int Xmax, Ymax, Zmax;
        int size;
        double progress;
        double d_REVsizes = (double) this->REVsizes;

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
            
            Xmax = (this->imageWidth - size);
            Ymax = (this->imageHeight - size);
            Zmax = (this->imageDepth - size);

            mRNG rngX(0, Xmax);
            mRNG rngY(0, Ymax);
            mRNG rngZ(0, Zmax);            

            for(int revIdx = 0; revIdx < localREVs; revIdx++)
            { 
                porositySum += (*this).computePorosity(rngX(), rngY(), rngZ(), size, REVVolume);        
            }

            this->REVporositiesData[sIdx] = porositySum / (double) localREVs;

            progress = (((double) sIdx + 1.0) / d_REVsizes);
            cout << "\r" << "Progress: " << std::setprecision(5) << 100.0 * progress << "%          " << std::flush;            
        }

        cout << "\r                                        \r" << std::flush;
    }

    void runCentralAnalysis()
    {
        cout << "- Running REV analysis by Centered method" << endl;
        double porositySum;
        double REVVolume;
        int Xcen, Ycen, Zcen;
        int Xmin, Ymin, Zmin;
        int size;
        double progress;
        double d_REVsizes = (double) this->REVsizes;

        // Find image central point coordinates
        Xcen = this->imageWidth / 2;
        Ycen = this->imageHeight / 2;
        Zcen = this->imageDepth / 2;

        for (int sIdx = 0; sIdx < this->REVsizes; sIdx++)
        {
            porositySum = 0.0;
            size = this->REVsizesData[sIdx];
            REVVolume = pow((double) size, 3.0);
            int localREVs = 1;
            if(localREVs > this->maxREVsPerSize) 
            { 
                   localREVs = maxREVsPerSize;
            }
          
            Xmin = Xcen - (size / 2);
            Ymin = Ycen - (size / 2);
            Zmin = Zcen - (size / 2);

            this->REVporositiesData[sIdx] = (*this).computePorosity(Xmin, Ymin, Zmin, size, REVVolume);

            progress = (((double) sIdx + 1.0) / d_REVsizes);
            cout << "\r" << "Progress: " << std::setprecision(5) << 100.0 * progress << "%          " << std::flush;            
        }

        cout << "\r                                        \r" << std::flush;
    }

    double computePorosity(int x0, int y0, int z0, int dim, double volume)
    {
        int xF = x0 + dim;
        int yF = y0 + dim;
        int zF = z0 + dim;
        unsigned long int arrayPos;
        unsigned long int poreVolume = 0;

        for(unsigned long int z = z0; z < zF; z++)
        {
            for (unsigned long int y = y0; y < yF; y++)
            {

                for (unsigned long int x = x0; x < xF; x++)
                {
                    arrayPos = IDX2C_3D(x, y, z, (unsigned long int) this->imageWidth, (unsigned long int) this->imageHeight);
                    poreVolume += (unsigned long int) this->imageData[arrayPos];
                }
            }
        }

        return ((double) poreVolume / volume);
    }
};

#endif