#ifndef IMAGEFILEPARSER_H
#define IMAGEFILEPARSER_H

// include Configuration file
#include "ImageREVHeader.h"

using namespace std;

class ImageFileParser
{
private:
    string filePath;
    int digits;
    int first;
    int count;
    int last;
    int current;
    string extension;
public:
    ImageFileParser(string filePath, int digits, int first, int count, string extension)
    {
        this->filePath = filePath;
        this->digits = digits;
        this->first = first;
        this->count = count;
        this->current = first;
        this->last = first + count;
        this->extension = extension;
    }

    virtual ~ImageFileParser()
    {}

    string getfilePath() { return this->filePath; }
    string getExtension() { return this->extension; }
    int getDigits() { return this->digits; }
    int getFirst() { return this->first; }
    int getCount() { return this->count; }
    int getLast() { return this->last; }
    int getCurrent() { return this->current; }

    string next()
    {	
    	if(current < this->last)
	    {
	    	string file = (this->filePath + (*this).convertCurrentIdxToString() + this->extension);
	    	this->current++;
	        return file;
	    } else
	    {
	    	throw std::invalid_argument("end of list was reached");
	    }

    }

    void setCurrent(int idx)
    {
    	this->current = idx;
    }

    void reset()
    {
    	this->current = this->first;
    }

    string convertCurrentIdxToString()
    {
        stringstream result;
        result << std::setfill('0') << std::setw(this->digits) << this->current;
        return result.str();
    }
};

#endif