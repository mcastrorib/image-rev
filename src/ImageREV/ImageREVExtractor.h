#ifndef IMAGEREVEXTRACTOR_H_
#define IMAGEREVEXTRACTOR_H_

// include Configuration file
#include "ImageREVHeader.h"

#define REV_PORE_COLOR 0
#define REV_MATRIX_COLOR 255

using namespace std;

class ImageREVExtractor
{
private:
	ImageREVAnalyzer *rev;
	string folderName;
	string folderPath;
	string pyscript;
	int X0;
	int XF;
	int Y0;
	int YF;
	int Z0;
	int ZF;
	int size;

public:
	ImageREVExtractor()
	{
		this->rev = NULL;
		this->folderName = "";
		this->folderPath = "";
		this->pyscript = "";
		this->X0 = 0;
		this->XF = 0;
		this->Y0 = 0;
		this->YF = 0;
		this->Z0 = 0;
		this->ZF = 0;
		this->size = 0;
	}

	ImageREVExtractor(ImageREVAnalyzer *_rev, string _folderName, string _folderPath, string _pyscript)
	{
		this->rev = _rev;
		this->folderName = _folderName;
		this->folderPath = _folderPath;
		this->pyscript = _pyscript;
		this->X0 = 0;
		this->XF = 0;
		this->Y0 = 0;
		this->YF = 0;
		this->Z0 = 0;
		this->ZF = 0;
		this->size = 0;
	}

	ImageREVExtractor(const ImageREVExtractor &other)
	{
		this->rev = other.rev;
		this->folderName = other.folderName;
		this->folderPath = other.folderPath;
		this->pyscript = other.pyscript;
		this->X0 = other.X0;
		this->XF = other.XF;
		this->Y0 = other.Y0;
		this->YF = other.YF;
		this->Z0 = other.Z0;
		this->ZF = other.ZF;
		this->size = other.size;
	}

	virtual ~ImageREVExtractor()
	{
		if(this->rev != NULL)
		{
			this->rev = NULL;
		}
	}

	void runExtraction()
	{
		string userInput;
		cout << "- Do you want to extract REV from original image? (y/n): ";
		cin >> userInput;

		if(userInput == "y")
		{
			(*this).setExtractionSize();
			(*this).checkValidSize();
			(*this).createExtractionDir();
			(*this).extractREV(this->rev->getMethod());
		}
	}

	void showREVAnalysis()
	{
		if(this->rev->getValidAnalysis())
		{
			string str_cmd = "python3 " + this->pyscript + " " + this->folderPath;
			char char_cmd[str_cmd.length() + 1];
			strcpy(char_cmd, str_cmd.c_str());

			int cmd_assert = std::system(char_cmd);
		}	
	}

	void setExtractionSize()
	{			
		string userInput;
		cout << "Select REV size: ";
		cin >> userInput;
		this->size = std::stoi(userInput);
	}

	void checkValidSize()
	{
		if(this->size < 1)
		{
			this->size = 1;
		}
		else if(this->size > this->rev->getMaxREVSize())
		{
			this->size = this->rev->getMaxREVSize();
		}
	}

	void extractREV(string _method)
	{
		if(_method == "mc")
		{
			(*this).extractMonteCarloREV();
		} 
		else if(_method == "center")
		{
			(*this).extractCentralREV();
		} else
		{
			cout << "method not specified" << endl;
		}
	}

	void extractMonteCarloREV()
	{
		int digits = getRequiredDigits(this->size);
	}

	void extractCentralREV()
	{
		uchar *imageData = this->rev->getImageData();
		string extension = ".png";
		int digits = getRequiredDigits(this->size);
		cv::Mat image = cv::Mat::zeros(cv::Size(this->size, this->size),CV_8UC1);
		uchar *currentPixel;
		
		// Find original image central point coordinates
        int Xcen = this->rev->getImageWidth() / 2;
        int Ycen = this->rev->getImageHeight() / 2;
        int Zcen = this->rev->getImageDepth() / 2;

        // Setting REV cubic region coordinates 
        int X0 = Xcen - (this->size / 2);
        int Y0 = Ycen - (this->size / 2);
        int Z0 = Zcen - (this->size / 2);
        // int XF = X0 + this->size;
        // int YF = Y0 + this->size;
        // int ZF = Z0 + this->size;

        // Extract color info
        int arrayPos;
        int currentSlice = 0;
        for(int z = 0; z < this->size; z++)
        {
            for (int y = 0; y < this->size; y++)
            {

            	currentPixel = image.ptr<uchar>(y);
                for (int x = 0; x < this->size; x++)
                {
                    arrayPos = IDX2C_3D((X0 + x), (Y0 + y), (Z0 + z), this->rev->getImageWidth(), this->rev->getImageHeight());
                    if(imageData[arrayPos] == 1)
                    {
                    	currentPixel[x] = REV_PORE_COLOR;
                    } else
                    {
                    	currentPixel[x] = REV_MATRIX_COLOR;
                    }
                }

            }

            // Save and increment current slice 
            (*this).saveImageSlice(image, currentSlice, digits, extension);
            currentSlice++;
        }
	}

	int getRequiredDigits(int slices)
	{
		int rem = slices - 1;
		int digits = 1;

		while(rem / 10 > 0)
		{
			rem /= 10;
			digits++;
		}

		return digits;
	}

	void saveImageSlice(cv::Mat _image, int _slice, int _digits, string _extension)
    {
        string fileIdx = (*this).convertCurrentIdxToString(_slice, _digits);
        string fullpath = this->folderPath + "/imgs/" + this->folderName + "_" + fileIdx + _extension;
        cv::imwrite(fullpath, _image); 
    }

    string convertCurrentIdxToString(int idx, int digits)
    {
        stringstream result;
        result << std::setfill('0') << std::setw(digits) << idx;
        return result.str();
    }

	void createExtractionDir()
    {
        string path = this->folderPath + "/imgs";
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