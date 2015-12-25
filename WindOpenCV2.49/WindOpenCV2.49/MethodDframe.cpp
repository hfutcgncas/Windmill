#include "MethodDframe.h"

void MethodDframe::MainLoop()
{

	if (!capture.isOpened())
	{
		std::cout << "read video failure, init first" << std::endl;
		return;
	}

	int flagindex = 0;
	while (capture.read(frame))
	{
		frameNo++;

		
		if (flagindex<2)
			//初始化各帧
	#pragma region InitFrame
		{

			switch (flagindex)
			{
			case 0:
				framePro1 = frame.clone();
				break;
			case 1:
				framePro2 = framePro1.clone();
				framePro1 = frame.clone();
				break;
			}
			flagindex++;
#pragma endregion
		}
		else
		{
	#pragma region find_df
			//差帧-----------------
			GaussianBlur(frame, frame, Size(3, 3), 0, 0, BORDER_DEFAULT);

			absdiff(frame, framePro1, dframe1);
			absdiff(framePro1, framePro2, dframe2);

			framePro2 = framePro1.clone();
			framePro1 = frame.clone();

			cvtColor(dframe1, dframe1, CV_BGR2GRAY);
			cvtColor(dframe2, dframe2, CV_BGR2GRAY);
			threshold(dframe1, dframe1, 30, 255, CV_THRESH_BINARY);
			threshold(dframe2, dframe2, 30, 255, CV_THRESH_BINARY);
#pragma endregion
			// 速度较快时去重影
			cv::dilate(dframe2, dframe2, cv::Mat(), Point(-1, -1), 1);
			dframe1.setTo(0, dframe2);  //去除重影

			//提取轮廓中的长直线------------------------------
	#pragma region detectLine 
			//Canny(dframe1, dframe1, 30, 90);

			vector<vector<Point>> contours;
			// find 
			findContours(dframe1, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			// draw 
			Mat result(dframe1.size(), 0, Scalar(0));
			drawContours(result, contours, -1, Scalar(255), 2);
			namedWindow("contours");
			
#pragma endregion

			//轮廓聚类------------------------------
	#pragma region clusters 
			//vector<Vec4i> lines;
			//// 检测直线，最小投票为90，线条不短于50，间隙不小于10 
			//HoughLinesP(result, lines, 1, 3.14159265 / 180, 70, 100, 10);
			//drawDetectLines(frame, lines, Scalar(0, 255, 0));
			//namedWindow("Lines");
			//imshow("src", frame);

			Mat Labels;
			Mat centers = cluster(result, &Labels, 150);
#pragma endregion
			//计算角速度----------------------------------------

			w = w_calculator.calc(centers, rate);
			cout << "FrameNO " << frameNo << "  角速度：" << w << "r/min" << endl;

			imshow("dframe1", dframe1);
			imshow("contours", result);

			if (cv::waitKey(delaytime) > 0)
			{
				break;
			}
		}
	}
}