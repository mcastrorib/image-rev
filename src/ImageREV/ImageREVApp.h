#ifndef IMAGEREV_APP_H_
#define IMAGEREV_APP_H_

// include Configuration file
#include "ImageREVHeader.h"

using namespace std;

class ImageREVApp
{
private:
    string source;
    string folderName;
    bool validSource;
    vector<cv::Mat> sourceImages;
    int sourceImagesCount;
    vector<cv::Mat> REVImages;
    
    ImageREVInput *input;
    ImageFileParser *parser;
    ImageREVAnalyzer *rev;
    ImageREVExtractor *extractor;

public:
    ImageREVApp(string inputFile, string datavisPyscript)
    {
        cout << ">> Image REV app" << endl;
        vector<cv::Mat> sourceImages();
        vector<cv::Mat> REVImages();

        this->input = new ImageREVInput(inputFile);
        this->source = this->input->getOriginFilePath() + this->input->getOriginFileName();
        this->parser = new ImageFileParser(this->source, this->input->getDigits(), this->input->getFirst(), this->input->getCount(), this->input->getExtension());
        this->rev = new ImageREVAnalyzer();
        this->validSource = (*this).checkSourceImages();
        (*this).setFolderName();
        this->extractor = new ImageREVExtractor(this->rev, this->input->getDestinationFileName(), this->folderName, datavisPyscript);        
    }

    virtual ~ImageREVApp()
    {
        if(this->rev != NULL)
        {
            delete this->rev;
            this->rev = NULL;
        }

        if(this->extractor != NULL)
        {
            this->extractor = NULL;
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
        cout << "- Checking image data." << endl;
        string imageFile;
        cv::Mat image;
        int width;
        int height;
        bool quit = false;
        int imageIndex = 0;
        double nImages = (double) this->input->getCount();
        double progress;

        imageFile = this->parser->next();
        image = cv::Mat(cv::imread(imageFile, cv::IMREAD_GRAYSCALE));
        width = image.cols;
        height = image.rows;

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
                image = cv::Mat(cv::imread(imageFile, cv::IMREAD_GRAYSCALE));
                if(image.cols != width || image.rows != height)
                {
                    cout << "image list is not uniform" << endl;
                    return false;
                }

                
                // log progress
                imageIndex++;
                progress = (((double) imageIndex + 1.0) / nImages);
                cout << "\r" << "Progress: " << std::setprecision(5) << 100.0 * progress << "%          " << std::flush;

            } catch (const std::invalid_argument& e) 
            {
                quit = true;
            }
        } 

        cout << "\r                                        \r" << std::flush;
        this->parser->reset();
        return true;
    }


    void readSourceImages_old()
    {
        this->sourceImages.reserve(this->input->getCount());
        string imageFile;
        cv::Mat *image;
        bool quit = false;
        
        cout << "Reading image data from folder " << this->input->getOriginFilePath() << endl;
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

    void readSourceImages()
    {
        string imageFile;
        cv::Mat image;
        int imageIndex;
        double nImages = (double) this->input->getCount();
        double progress;
        bool quit = false;

        cout << "- Reading image data." << endl; // from folder " << this->input->getOriginFilePath() << endl;

        // Reading 1st image
        imageFile = this->parser->next();
        image = cv::Mat(cv::imread(imageFile, cv::IMREAD_GRAYSCALE));
        if (!image.data)
        {
            cout << "Error: No image data in file " << imageFile << endl;
            quit = true;
        } else  
        {
            this->rev->checkFirstImage(image, this->input->getREVSizes());
            this->rev->setArrays();
            this->rev->readImage(image, imageIndex);
            imageIndex++;
        }

        
        while(!quit)
        {
            try 
            {
                imageFile = this->parser->next();
                image = cv::Mat(cv::imread(imageFile, cv::IMREAD_GRAYSCALE));
                if (!image.data)
                {
                    cout << "Error: No image data in file " << imageFile << endl;
                    quit = true;
                } else  
                {
                    this->rev->readImage(image, imageIndex);
                    imageIndex++;

                    // log progress
                    progress = (((double) imageIndex + 1.0) / nImages);
                    cout << "\r" << "Progress: " << std::setprecision(5) << 100.0 * progress << "%          " << std::flush;  
                }
            } catch (const std::invalid_argument& e) 
            {
                // std::cerr << "exception: " << e.what() << std::endl; 
                quit = true;
            }
        } 

        cout << "\r                                        \r" << std::flush;
        this->parser->reset();
        this->sourceImagesCount = imageIndex + 1;
        
    }

    void run_old()
    {
        if(this->validSource)
        {
            (*this).readSourceImages_old();
            this->rev->set_old(this->sourceImages, 
                               this->input->getREVMethod(), 
                               this->input->getPorePhaseColor(),
                               this->input->getREVSizes(),
                               this->input->getMaxREVSamples());
            this->rev->runAnalysis();
            (*this).save();
        }
    }

    void run()
    {
        if(this->validSource)
        {
            this->rev->set(this->input->getCount(),
                           this->input->getREVMethod(), 
                           this->input->getPorePhaseColor(),
                           this->input->getMaxREVSamples());
            (*this).readSourceImages();

            if(this->input->getAnalysis()) 
            {
                this->rev->runAnalysis();
                (*this).save();
                
                // Visualize REV analysis
                if(this->input->getDatavis()) 
                    this->extractor->showREVAnalysis();
            }
            
            if(this->input->getExtract()) 
            {
                this->extractor->runExtraction();
            }            
        }
    }

    void save()
    {
        (*this).createDestinationDir();   
        (*this).saveAnalysis();
    }

    void saveREV()
    {
        string fullpath = this->input->getDestinationFilePath() + this->input->getDestinationFileName() + "/" + this->input->getDestinationFileName();
        int digits = (*this).findDigits(this->sourceImagesCount);
        for(int slice = 0; slice < this->sourceImagesCount; slice++)
        {
            (*this).saveImageSlice(fullpath, slice, digits, this->input->getExtension());
        }
    }

    void saveAnalysis()
    {
        string filepath1 = this->folderName + "/REV_analysis_info.txt"; 
        ofstream fileObject;
        fileObject.open(filepath1, ios::out);
        if (fileObject.fail())
        {
            cout << "Could not open file from disc." << endl;
            exit(1);
        }
        fileObject << "Max_REV_samples: " << this->rev->getMaxREVsPerSize() << endl;
        fileObject.close();

        string filepath2 = this->folderName + "/REV_analysis.csv";  
        fileObject.open(filepath2, ios::out);
        if (fileObject.fail())
        {
            cout << "Could not open file from disc." << endl;
            exit(1);
        }

        fileObject << "ID" << ",";
        fileObject << "REV_SIZE" << ",";
        fileObject << "MEAN_REV_POROSITY" << endl;
        for (uint revIdx = 0; revIdx < this->rev->getREVsizes(); revIdx++)
        {
            fileObject << revIdx << ",";
            fileObject << this->rev->getREVsizesData(revIdx) << ",";
            fileObject << this->rev->getREVporositiesData(revIdx) << endl;
        }
        fileObject.close();
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

    void setFolderName()
    {
        this->folderName = this->input->getDestinationFilePath() + this->input->getDestinationFileName();
        this->folderName = this->folderName + "_rm=" + this->input->getREVMethod();
        this->folderName = this->folderName + "_rs=" + std::to_string(this->input->getMaxREVSamples()); 
    }

    void createDestinationDir()
    {
        string path = this->folderName;
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