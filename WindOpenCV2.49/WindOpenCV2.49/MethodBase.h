#pragma once
#include"common.h"

class W_calculator
{
private:

	float Theata1[3];
	float Theata2[3];
	float dTheata[3];

	Mat CurrentDis;
	Mat Lastis;
	//========================================

	int disMarch(Mat CurrentTheata, Mat LastTheata, int len, int IndexTemp[]);

	Mat AdjustCurrentTheata(Mat CurrentTheata, Mat LastTheata, int len);

public:
	float w;
	float w_last;

	W_calculator();

	float calc(Mat CurrentDis, float frameRate);
	

};


class MethodBase
{
public:
	MethodBase();
	~MethodBase();

	float w = 0;
	bool initVideo(string videoFile);
protected:

	cv::VideoCapture capture;
	double rate;
	int delaytime;
	long frameNo;
	Mat frame;

	W_calculator w_calculator;

	//======================
	Mat cluster(Mat inPut, Mat *plabels, int HPthreshold, int clusterCount = 3);
};




