# image-rev

## Introduction
image-rev is an open-source application for micro tomographic images Representative Element Volume (REV) analysis based on porosity. As input, it receives a list of pre-proccessed binary images that identifies each phase (porous or matrix). The output is a simple .csv file with the measured porosity for specific REV sizes. In addition, user's can also extract a representative volume from the original image.   
Currently, image-rev runs only in linux and it is majorly written in C++ and requires [CMake](https://cmake.org/) 3.8 or greater for project build and compilation and [OpenCV](https://opencv.org/) for image preccessing. Also, some simple [Python](https://www.python.org/) scripts for data visualization that requires [Matplotlib](link).   

## Features
+ Representative Volume (REV) analysis of a 3D binary image;
+ Representative Volume (REV) extraction from a 3D binary image;

## Requirements

The following libraries are required:
+ [CMake 3.8 or greater](https://cmake.org/);
+ [OpenCV](https://opencv.org/);
+ [Python 3](https://www.python.org/);
+ [NumPy](https://numpy.org/);
+ [Pandas](https://pandas.pydata.org/);
+ [Matplotlib](https://matplotlib.org/).

## Installation
image-rev execution requires three steps: C++ requirements installation, Python requirements installation and Project build/compilation.

For the C++ requirements, first update the package manager repositories. E.g., for Ubuntu:
'''
sudo apt update
'''  

If an older version of Cmake is already installed, first unninstall it with:
'''
sudo apt --purge autoremove cmake
'''

Now install CMake and OpenCV with commands
'''
sudo snap install cmake --classic
sudo apt install libopencv-dev python3-opencv
'''

For the python requirements, we recommend using pip3 as:
'''
pip3 install numpy
pip3 install pandas
pip3 install matplotlib
'''

Now that all the requirements are installed, run the bash script ''install.sh'' for building the project and compiling the image-rev executable
'''
./install.sh
''' 

## How to run