#ifndef panoManipulation_h
#define panoManipulation_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <chrono>
#include "opencv4/opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define START_WIDTH  6000
#define START_HEIGHT 3000
#define WIDTH        6000
#define HEIGHT       3000

typedef struct pixel pixel;
struct pixel
{
    float x;
    float y;
};

string ConvertSectoDay(int n);
pixel computeDewarping(float yaw, float pitch, float roll, pixel p);

#endif