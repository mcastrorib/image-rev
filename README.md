# image-rev

## Introduction
**image-rev** is an open-source application for micro tomographic images Representative Element Volume (REV) analysis based on porosity. As input, it receives a list of pre-proccessed binary images that identifies each phase (porous or matrix). The output is a simple .csv file with the measured porosity for specific REV sizes. In addition, user's can also extract a representative volume from the original image. Currently, **image-rev** runs only in linux and it is majorly written in C++ and requires [CMake](https://cmake.org/) 3.8 or greater for project build and compilation and [OpenCV](https://opencv.org/) for image preccessing. Also, some simple [Python](https://www.python.org/) scripts for data visualization that requires [Matplotlib](https://matplotlib.org/).   

## Features
+ Representative Volume (REV) analysis of a 3D binary image;
+ Representative Volume (REV) extraction from a 3D binary image;

## Requirements

The following software and libraries are required:
+ [CMake (3.8 or greater)](https://cmake.org/);
+ [OpenCV](https://opencv.org/);
+ [Python 3](https://www.python.org/);
+ [NumPy](https://numpy.org/);
+ [Pandas](https://pandas.pydata.org/);
+ [Matplotlib](https://matplotlib.org/).

## Installation
**image-rev** execution requires three steps: C++ requirements installation, Python requirements installation and Project build/compilation.

For the C++ requirements, update the package manager repositories. E.g., for Ubuntu distro:
```
sudo apt update
``` 

Now install CMake and OpenCV via apt:
```
sudo snap install cmake --classic
sudo apt install libopencv-dev python3-opencv
```

For the Python requirements, we recommend using pip3 as:
```
pip3 install numpy
pip3 install pandas
pip3 install matplotlib
```

Now that all the requirements are installed, run the bash script ``install.sh`` at the project root directory for building the project and compiling the **image-rev** executable:  
```
./install.sh
```

## How to run

After the **image-rev** application is succesfully installed, it can be run via the ``image-rev`` executable at the project root directory. The application input configuration is controlled via the ``image.conf`` file located at the ``config`` directory. This is a simple text editable file where the required inputs are organized in columns:

**ORIGIN_PATH:** (string) path to the directory where input images files are saved;

**ORIGIN_FILE:** (string) input image files prefix;

**EXTENSION:** (string) input image files extension;

**DIGITS:** (integer) number of numeric digits identifying the input image files;

**FIRST:** (integer) first input image file index;

**COUNT:** (integer) number of input image files;

**PORE_PHASE_COLOR:** (integer) numeric value identifying the porous phase in the input image files;


**DESTINATION_PATH:** (string) path to the directory where results will be saved;

**DESTINATION_FILE:** (string) name of the folder where results will be saved;

**ANALYSIS:** (boolean) REV analysis flag; 

**DATAVIS:** (boolean) REV analysis visualization flag;

**EXTRACT:** (boolean) REV extraction flag;

**REV_METHOD:** (string) REV analysis method, it can be 'central' (REV grows from central cubic) or 'mc' (growing REVs are choosen 
randomly throughout the volume);

**REV_SIZES:** (integer) number of REV sizes used in the analysis;

**MAX_REV_SAMPLES:** (integer) number of REV samples used for each REV size (only used for 'mc' analysis).
