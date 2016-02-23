#include "MethodGMM.h"


MethodGMM::MethodGMM()
{
	
}

MethodGMM::~MethodGMM()
{
}

void MethodGMM::MainLoop()
{
	bg_model = BackgroundSubtractorMOG2(200, 16, true);

	if (!capture.isOpened())
	{
		std::cout << "read video failure, init first" << std::endl;
		return ;
	}
	while (capture.read(frame))
	{
		++frameNo;

		//找到前景----------------------------------------
		#pragma region GMM
		// 运动前景检测，并更新背景  
		//mog->apply(frame, foreground, 0.01);

		bg_model(frame, foreground, learnRate);

		// 去除孤立噪点
		// 腐蚀  
		cv::erode(foreground, foreground, cv::Mat(), Point(-1, -1), ErodeStrength);
		// 膨胀  
		cv::dilate(foreground, foreground, cv::Mat(), Point(-1, -1), ErodeStrength);

		// 返回当前背景图像  
		bg_model.getBackgroundImage(background);

		threshold(foreground, foreground, 100, 255, THRESH_BINARY);
		GaussianBlur(foreground, foreground, Size(3, 3), 0, 0, BORDER_DEFAULT);

		cv::imshow("video", foreground);
		//cv::dilate(foreground, foreground, cv::Mat(), Point(-1, -1), 10);
		//	cv::imshow("background", background);

#pragma endregion

		//提取轮廓中的长直线------------------------------
		#pragma region detectLine 
		vector<vector<Point>> contours;
		//// find 
		findContours(foreground, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		//findContours(foreground, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
		// draw 
		Mat result(foreground.size(), 0, Scalar(0));
		drawContours(result, contours, -1, Scalar(255), 2);
		
		
		////=======================================
		//snake方法
		//vector<Point>   InitContour;
		//CvPoint* point = new CvPoint[2000]; //分配轮廓点
		//int i = 0;
		//while (!contours.empty())
		//{
		//	InitContour = contours.back();
		//	contours.pop_back();
		//	while (!InitContour.empty())
		//	{
		//		point[i].x = InitContour.back().x;
		//		point[i].y = InitContour.back().y;
		//		InitContour.pop_back();
		//		i++;
		//		if (i > 1999)break;
		//	}
		//	if (i > 1999)break;
		//}
		//if (i < 10)
		//{
		//	cout << "too little" << endl;
		//}
		//else
		//{
		//	Mat	frame1;
		////	cvtColor(frame, frame1, CV_BGR2GRAY);
		//	Canny(frame, frame1, 70, 150);
		//	threshold(frame1, frame1, 100, 255, CV_THRESH_OTSU);
		//	IplImage ipl_img(frame1);  //(1) 
		//
		//
		//float alpha = 1 / 100.0f;
		//float beta = 1 / 100.0f;
		//float gamma = 50 / 100.0f;
		//CvSize size;
		//size.width = 3;
		//size.height = 3;
		//CvTermCriteria criteria;
		//criteria.type = CV_TERMCRIT_ITER;
		//criteria.max_iter = 1000;
		//criteria.epsilon = 0.1;
		//cvSnakeImage(&ipl_img, point, i, &alpha, &beta, &gamma, CV_VALUE, size, criteria, 0);
		//
		//int length = i;
		//for (int i = 0; i<length; i++)
		//{
		//	circle(frame, point[i], 2, Scalar(0, 0, 255), CV_FILLED, CV_AA);
		//}
		//delete[]point;
		//imshow("asd", frame);
		//}

#pragma endregion

		//轮廓聚类------------------------------
		#pragma region clusters 
		//轮廓聚类----------------------------------------
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


		imshow("foreground", frame);
		imshow("contours", result);
		if (cv::waitKey(1) > 0)
		{
			break;
		}
	}


}
