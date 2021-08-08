#ifndef IMAGEREV_HEADER_H_
#define IMAGEREV_HEADER_H_

// include C++ standard libraries
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <limits>
#include <random>
#include <omp.h>

// include opencv libraries
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>

// define macros
#define IDX2C_3D(x, y, z, lx, ly) ((x) + (y * lx) + (z * (lx * ly)))
#define PRINT_DATA false

// include ImageREV lib classes
#include "mRNG.h"
#include "ImageREV.h"
#include "ImageFileParser.h"
#include "ImageREVInput.h"
#include "ImageREVApp.h"

#endif