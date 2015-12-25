#include"common.h"
#include "MethodGMM.h"
#include"MethodDframe.h"


enum  Method
{
	GMM,
	dFRAME
};


int main(int argc, char** argv)
{
	MethodGMM methodGMM;
	MethodDframe methodDframe;

	string VideoPath = "./test.MOV";

	switch (dFRAME)
	{
	case GMM:	
		if (methodGMM.initVideo(VideoPath))
		{
			methodGMM.MainLoop();
		}		
		break;
	case dFRAME:
		if (methodDframe.initVideo(VideoPath))
		{
			methodDframe.MainLoop();
		}	
		break;
	}
	

	
	return 0;
}
