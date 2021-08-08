#ifndef IMAGEREVINPUT_H_
#define IMAGEREVINPUT_H_

// include Configuration file
#include "ImageREVHeader.h"

using namespace std;

class ImageREVInput
{
private:
    string inputFile;
    string originFilePath;
    string originFileName;
    string destinationFilePath;
    string destinationFileName;
    string extension;
    int digits;
    int first;
    int count;
    string REVMethod;
    uchar porePhaseColor;

public:
    ImageREVInput(const string inputFile)
    {
        this->inputFile = inputFile;
        (*this).readInputFile();
    }

    ImageREVInput(const ImageREVInput &other)
    {
        this->inputFile = other.inputFile;
        this->originFilePath = other.originFilePath;
        this->originFileName = other.originFileName;
        this->destinationFilePath = other.destinationFilePath;
        this->destinationFileName = other.destinationFileName;
        this->extension = other.extension;
        this->digits = other.digits;
        this->first = other.first;
        this->count = other.count;
        this->REVMethod = other.REVMethod;
        this->porePhaseColor = other.porePhaseColor;
    }

    virtual ~ImageREVInput()
    {}

    string getOriginFilePath() { return this->originFilePath; }
    string getOriginFileName() { return this->originFileName; }
    string getDestinationFilePath() { return this->destinationFilePath; }
    string getDestinationFileName() { return this->destinationFileName; }
    string getExtension() { return this->extension; }
    int getDigits() { return this->digits; }
    int getFirst() { return this->first; }
    int getCount() { return this->count; }
    string getREVMethod() { return this->REVMethod; }
    uchar getPorePhaseColor() { return this->porePhaseColor; }



    void readInputFile()
    {
        ifstream fileObject;
        fileObject.open(this->inputFile, ios::in);
        if (fileObject.fail())
        {
            cout << "Could not open uct config file from disc." << endl;
            exit(1);
        }

        string line;
        while(fileObject)
        {
            getline(fileObject, line);
            // cout << line << endl;

            string s = line;
            string delimiter = ": ";
            size_t pos = 0;
            string token, content;
            while ((pos = s.find(delimiter)) != std::string::npos) 
            {
                token = s.substr(0, pos);
                content = s.substr(pos + delimiter.length(), s.length());
                s.erase(0, pos + delimiter.length());

                if(token == "ORIGIN_PATH") (*this).readOriginFilePath(content);
                else if(token == "ORIGIN_FILE") (*this).readOriginFileName(content);
                else if(token == "DESTINATION_PATH") (*this).readDestinationFilePath(content);
                else if(token == "DESTINATION_FILE") (*this).readDestinationFileName(content);
                else if(token == "EXTENSION") (*this).readExtension(content);
                else if(token == "DIGITS") (*this).readDigits(content);
                else if(token == "FIRST") (*this).readFirst(content);
                else if(token == "COUNT") (*this).readCount(content);
                else if(token == "REV_METHOD") (*this).readREVMethod(content);
                else if(token == "PORE_PHASE_COLOR") (*this).readPorePhaseColor(content);                          
            }
        } 

        fileObject.close();
    }

    void readOriginFilePath(string content)
    {
        this->originFilePath = content;
    }

    void readOriginFileName(string content)
    {
        this->originFileName = content;
    }

    void readDestinationFilePath(string content)
    {
        this->destinationFilePath = content;
    }

    void readDestinationFileName(string content)
    {
        this->destinationFileName = content;
    }

    void readExtension(string content)
    {
        this->extension = content;
    }

    void readDigits(string content)
    {
        this->digits = std::stoi(content);
    }

    void readFirst(string content)
    {
        this->first = std::stoi(content);
    }

    void readCount(string content)
    {
        this->count = std::stoi(content);
    }

    void readREVMethod(string content)
    {
        this->REVMethod = content;
    }

    void readPorePhaseColor(string content)
    {
        this->porePhaseColor = (uchar) std::stoi(content);
    }
};

#endif