#pragma once
#include "MethodBase.h"

class MethodGMM :
	public MethodBase
{
	cv::Mat foreground;
	cv::Mat background;
	

	BackgroundSubtractorMOG2 bg_model;

public:
	MethodGMM();
	~MethodGMM();

	void MainLoop();

private:
	float learnRate = 0.01; //背景学习率
	int ErodeStrength = 4;  //噪点抑制强度



};

