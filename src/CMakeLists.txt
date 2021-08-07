cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# set the project name
project(IMAGEREV VERSION 0.1 LANGUAGES C CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_IMAGEREV "Use provided ImageREV internal lib implementation" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(imagerev_config.h.in imagerev_config.h)

# include OpenCV package
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# include OpenMP package
find_package( OpenMP REQUIRED )

if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# list in-house libraries
if(USE_IMAGEREV)
  add_subdirectory(ImageREV)
  list(APPEND EXTRA_LIBS ImageREV)
endif()

# add the executable
add_executable(image-rev main.cpp)

target_link_libraries(image-rev PUBLIC ${EXTRA_LIBS} ${OpenCV_LIBS} OpenMP::OpenMP_CXX)


# add the binary tree to the search path for include files
# so that we will find *config.h
target_include_directories(image-rev PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
install(TARGETS image-rev DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/imagerev_config.h"
  DESTINATION include
  )

