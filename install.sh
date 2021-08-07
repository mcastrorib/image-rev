#!/bin/bash
# This scrip builds the ImageREV program usin CMAKE

# Get system available processors
procs=$(nproc --all)
echo "Building IMAGE_REV with ${procs} processors..."

# Source code path
SRC_DIR='./src/'

## BUILDING RELEASE VERSION
# Build directories path
RELEASE_DIR='./build/release'
# Configure the build
cmake -S ${SRC_DIR} -B ${RELEASE_DIR} -D CMAKE_BUILD_TYPE=Release
# Actually build the binaries
cmake --build build/release -j${procs}
# Create symbolic link in root directory
ln -sf ${RELEASE_DIR}/image-rev image-rev

## BUILDING DEBUG VERSION
# Build directories path
DEBUG_DIR='./build/debug'
# Configure the build
cmake -S ${SRC_DIR} -B ${DEBUG_DIR} -D CMAKE_BUILD_TYPE=Debug
# Actually build the binaries
cmake --build build/debug -j${procs}



