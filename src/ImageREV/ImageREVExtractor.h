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
	int size;

public:
	ImageREVExtractor()
	{
		this->rev = NULL;
		this->folderName = "";
		this->folderPath = "";
		this->pyscript = "";
		this->size = 0;
	}

	ImageREVExtractor(ImageREVAnalyzer *_rev, string _folderName, string _folderPath, string _pyscript)
	{
		this->rev = _rev;
		this->folderName = _folderName;
		this->folderPath = _folderPath;
		this->pyscript = _pyscript;
		this->size = 0;
	}

	ImageREVExtractor(const ImageREVExtractor &other)
	{
		this->rev = other.rev;
		this->folderName = other.folderName;
		this->folderPath = other.folderPath;
		this->pyscript = other.pyscript;
		this->size = other.size;
	}

	virtual ~ImageREVExtractor()
	{
		if(this->rev != NULL)
		{
			this->rev = NULL;
		}
	}

	uchar convertRGBtoGrayscale(uchar red, uchar green, uchar blue)
	{
	    return ((red * 7 / 255) << 5) + ((green * 7 / 255) << 2) + (blue * 3 / 255);   
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
		cout << "- Select REV size: ";
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
		cout << "- Extracting random REV with size " << this->size << "." << endl;
		
		// Define log variables
		double progress;
		double dSize = (double) this->size;

		// Define utils
		uchar *imageData = this->rev->getImageData();
		string extension = ".png";
		int digits = getRequiredDigits(this->size);
		cv::Mat image = cv::Mat::zeros(cv::Size(this->size, this->size),CV_8UC1);
		uchar *currentPixel;
		
		// Find original image max point coordinates
        unsigned long int Xmax = (unsigned long int) this->rev->getImageWidth() - this->size;
        unsigned long int Ymax = (unsigned long int) this->rev->getImageHeight() - this->size;
        unsigned long int Zmax = (unsigned long int) this->rev->getImageDepth() - this->size;

        // Randomly setting REV cubic region initial coordinates 
        mRNG rngX(0, Xmax);
        mRNG rngY(0, Ymax);
        mRNG rngZ(0, Zmax);  
        unsigned long int X0 = (unsigned long int) rngX();
        unsigned long int Y0 = (unsigned long int) rngY();
        unsigned long int Z0 = (unsigned long int) rngZ();
        cout << "Extracting REV from {" << X0 << ", " << Y0 << ", " << Z0 << "}";
        cout << "to {" << X0+this->size << ", " << Y0+this->size << ", " << Z0+this->size << "}" << endl;

        // Extract color info
        unsigned long int arrayPos;
        int currentSlice = 0;
        for(unsigned long int z = 0; z < this->size; z++)
        {
            for (unsigned long int y = 0; y < this->size; y++)
            {

            	currentPixel = image.ptr<uchar>(y);
                for (unsigned long int x = 0; x < this->size; x++)
                {
                    arrayPos = IDX2C_3D((X0 + x), (Y0 + y), (Z0 + z), (unsigned long int) this->rev->getImageWidth(), (unsigned long int) this->rev->getImageHeight());
                    currentPixel[x] = imageData[arrayPos];
                    // if(imageData[arrayPos] == 1)
                    // {
                    // 	currentPixel[x] = REV_PORE_COLOR;
                    // } else
                    // {
                    // 	currentPixel[x] = REV_MATRIX_COLOR;
                    // }
                }

            }

            // Save and increment current slice 
            (*this).saveImageSlice(image, currentSlice, digits, extension);
            currentSlice++;

            progress = (((double) currentSlice) / dSize);
            cout << "\r" << "Progress: " << std::setprecision(5) << 100.0 * progress << "%          " << std::flush;            
        }

        // flush last progress line
        cout << "\r                                        \r" << std::flush; 
	}

	void extractCentralREV()
	{
		cout << "- Extracting central REV with size " << this->size << "." << endl;

		// Define log variables
		double progress;
		double dSize = (double) this->size;

		// Define utils
		uchar *imageData = this->rev->getImageData();
		string extension = ".png";
		int digits = getRequiredDigits(this->size);
		cv::Mat image = cv::Mat::zeros(cv::Size(this->size, this->size),CV_8UC1);
		uchar *currentPixel;
		
		// Find original image central point coordinates
        unsigned long int Xcen = (unsigned long int) this->rev->getImageWidth() / 2;
        unsigned long int Ycen = (unsigned long int) this->rev->getImageHeight() / 2;
        unsigned long int Zcen = (unsigned long int) this->rev->getImageDepth() / 2;

        // Setting REV cubic region coordinates 
        unsigned long int X0 = (unsigned long int) Xcen - (this->size / 2);
        unsigned long int Y0 = (unsigned long int) Ycen - (this->size / 2);
        unsigned long int Z0 = (unsigned long int) Zcen - (this->size / 2);

        // Extract color info
        unsigned long int arrayPos;
        int currentSlice = 0;
        for(unsigned long int z = 0; z < this->size; z++)
        {
            for (unsigned long int y = 0; y < this->size; y++)
            {

            	currentPixel = image.ptr<uchar>(y);
                for (unsigned long int x = 0; x < this->size; x++)
                {
                    arrayPos = IDX2C_3D((X0 + x), (Y0 + y), (Z0 + z), (unsigned long int) this->rev->getImageWidth(), (unsigned long int) this->rev->getImageHeight());
                    currentPixel[x] = imageData[arrayPos];
                    // if(imageData[arrayPos] == 1)
                    // {
                    // 	currentPixel[x] = REV_PORE_COLOR;
                    // } else
                    // {
                    // 	currentPixel[x] = REV_MATRIX_COLOR;
                    // }
                }

            }

            // Save and increment current slice 
            (*this).saveImageSlice(image, currentSlice, digits, extension);
            currentSlice++;
        	
        	progress = (((double) currentSlice) / dSize);
            cout << "\r" << "Progress: " << std::setprecision(5) << 100.0 * progress << "%          " << std::flush;            
        }

        // flush last progress line
        cout << "\r                                        \r" << std::flush; 
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