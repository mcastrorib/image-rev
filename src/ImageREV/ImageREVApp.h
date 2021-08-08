#ifndef IMAGEREV_APP_H_
#define IMAGEREV_APP_H_

// include Configuration file
#include "ImageREVHeader.h"

using namespace std;

class ImageREVApp
{
private:
    string source;
    bool validSource;
    vector<cv::Mat> sourceImages;
    int sourceImagesCount;
    vector<cv::Mat> REVImages;
    
    ImageREVInput *input;
    ImageFileParser *parser;
    ImageREV *rev;

public:
    ImageREVApp(string inputFile)
    {
        vector<cv::Mat> sourceImages();
        vector<cv::Mat> REVImages();

        this->input = new ImageREVInput(inputFile);
        this->source = this->input->getOriginFilePath() + this->input->getOriginFileName();
        this->parser = new ImageFileParser(this->source, this->input->getDigits(), this->input->getFirst(), this->input->getCount(), this->input->getExtension());
        this->rev = new ImageREV();
        this->validSource = (*this).checkSourceImages();
    }

    virtual ~ImageREVApp()
    {
        if(this->rev != NULL)
        {
            delete this->rev;
            this->rev = NULL;
        }

        if(this->input != NULL)
        {
            delete this->input;
            this->input = NULL;
        }

        if(this->parser != NULL)
        {
            delete this->parser;
            this->parser = NULL;
        }
    }

    bool checkSourceImages()
    {
        /* 
            Check if origin images are not empty or have uniform resolutions
        */
        string imageFile;
        cv::Mat *image;
        int width;
        int height;
        bool quit = false;

        imageFile = this->parser->next();
        image = new cv::Mat(cv::imread(imageFile));
        width = image->cols;
        height = image->rows;

        if(width == 0 || height == 0 || this->parser->getCount() == 0)
        {
            cout << "file: " << imageFile << "is an empty image" << endl;
            return false;
        }

        while(!quit)
        {
            try 
            {
                imageFile = this->parser->next();
                image = new cv::Mat(cv::imread(imageFile));
                if(image->cols != width || image->rows != height)
                {
                    cout << "image list is not uniform" << endl;
                    return false;
                }
            } catch (const std::invalid_argument& e) 
            {
                std::cerr << "exception: " << e.what() << std::endl; 
                quit = true;
            }
        } 
        this->parser->reset();
        return true;
    }


    void readSourceImages()
    {
        string imageFile;
        cv::Mat *image;
        bool quit = false;

        while(!quit)
        {
            try 
            {
                imageFile = this->parser->next();
                image = new cv::Mat(cv::imread(imageFile));
                if (!image->data)
                {
                    cout << "Error: No image data in file " << imageFile << endl;
                    quit = true;
                } else  
                {
                    this->sourceImages.push_back(image->clone());
                }
            } catch (const std::invalid_argument& e) 
            {
                std::cerr << "exception: " << e.what() << std::endl; 
                quit = true;
            }
        } 
        this->parser->reset();
        this->sourceImagesCount = this->sourceImages.size();
    }

    void run()
    {
        if(this->validSource)
        {
            (*this).readSourceImages();
            this->rev->set(this->sourceImages, this->input->getREVMethod(), this->input->getPorePhaseColor());
            this->rev->runAnalysis();
        }
    }

    void save()
    {
        (*this).createDestinationDir();   
        string fullpath = this->input->getDestinationFilePath() + this->input->getDestinationFileName() + "/" + this->input->getDestinationFileName();
        int digits = (*this).findDigits(this->sourceImagesCount);
        for(int slice = 0; slice < this->sourceImagesCount; slice++)
        {
            (*this).saveImageSlice(fullpath, slice, digits, this->input->getExtension());
        }
    }

    int findDigits(int n)
    {
        int count = 1;
        int result = n / 10;

        while (result > 0)
        {
            count++;
            result = result / 10;
        }

        return count;
    }

    void saveImageSlice(string path, int slice, int digits, string extension)
    {
        string fileIdx = (*this).convertCurrentIdxToString(slice, digits);
        string fullpath = path + "_" + fileIdx + extension;
        cv::imwrite(fullpath, this->REVImages[slice]); 
    }

    string convertCurrentIdxToString(int idx, int digits)
    {
        stringstream result;
        result << std::setfill('0') << std::setw(digits) << idx;
        return result.str();
    }

    void createDestinationDir()
    {
        string path = this->input->getDestinationFilePath() + this->input->getDestinationFileName();
        char directory[path.size() + 1];
        strcpy(directory, path.c_str());

        struct stat st = {0};

        if (stat(directory, &st) == -1)
        {
            mkdir(directory, 0700);
        } 
    }
};

#endif