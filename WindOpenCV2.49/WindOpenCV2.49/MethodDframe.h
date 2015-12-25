#pragma once
#include "MethodBase.h"
class MethodDframe :
	public MethodBase
{
public:
	void MainLoop();

private:

	Mat  framePro1, framePro2, dframe1, dframe2, ddframe;
	
};

