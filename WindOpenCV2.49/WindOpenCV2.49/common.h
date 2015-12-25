#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

#include <cxcore.hpp>

//-----------------------------------------
#include <iostream>  
#include <string>  

#include<opencv\cv.h>
#include <opencv2/opencv.hpp>  
//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
#include "opencv2/core/core.hpp"

#include <algorithm>

#include <opencv2/legacy/legacy.hpp>


using namespace cv;
using namespace std;


void drawDetectLines(Mat& image, const vector<Vec4i>& lines, Scalar & color);


