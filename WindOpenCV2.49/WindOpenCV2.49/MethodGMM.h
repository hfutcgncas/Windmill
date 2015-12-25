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

};

