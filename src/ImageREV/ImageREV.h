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
    int imageHeigth;
    int imageDepth;
    int maxREVSize;
    double *meanPorosities;

public:
    ImageREV()
    {
        this->method = "None";
        this->validMethod = false;
        this->imageData = NULL;
        this->imageWidth = 0;
        this->imageHeigth = 0;
        this->imageDepth = 0;
        this->maxREVSize = 0;
        this->meanPorosities = NULL;
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
    }

    void set(vector<cv::Mat> &_sourceImages, string _method)
    {
        this->method = _method;
        (*this).checkMethod();
        (*this).checkImage(_sourceImages);
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
            this->imageHeight = _sourceImages[0].height;
            this->imageWidth = _sourceImages[0].width / _sourceImages[0].channels();
            (*this).setMaxREVSize(this->imageWidth, this->imageHeight, this->imageDepth);
            (*this).setImageDataArray();
            (*this).setMeanPorosityArray();            
        }
    }

    void convertImageData(vector<cv::Mat> &_sourceImages)
    {}

    void setMaxREVSize(int width, int height, int depth)
    {
        int max = width;
        if(heigth > max) max = height;
        if(depth > max) max = depth;
        this->maxREVSize = max;
    }
};

#endif