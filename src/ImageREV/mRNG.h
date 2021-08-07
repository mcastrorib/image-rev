#ifndef RNG_H_
#define RNG_H_

#include "ImageREVHeader.h"

class mRNG {
    std::mt19937 eng;
    std::uniform_int_distribution<> distr;
public:
    mRNG(int lower, int upper, int threadID = 0) 
        : eng(std::random_device()() * (1 + threadID))
        , distr(lower, upper)
    {}

    int operator()() { 
         return distr(eng);
    }
};

#endif