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
			//��ʼ����֡
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
			//��֡-----------------
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
			// �ٶȽϿ�ʱȥ��Ӱ
			cv::dilate(dframe2, dframe2, cv::Mat(), Point(-1, -1), 1);
			dframe1.setTo(0, dframe2);  //ȥ����Ӱ

			//��ȡ�����еĳ�ֱ��------------------------------
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

			//��������------------------------------
	#pragma region clusters 
			//vector<Vec4i> lines;
			//// ���ֱ�ߣ���СͶƱΪ90������������50����϶��С��10 
			//HoughLinesP(result, lines, 1, 3.14159265 / 180, 70, 100, 10);
			//drawDetectLines(frame, lines, Scalar(0, 255, 0));
			//namedWindow("Lines");
			//imshow("src", frame);

			Mat Labels;
			Mat centers = cluster(result, &Labels, 150);
#pragma endregion
			//������ٶ�----------------------------------------

			w = w_calculator.calc(centers, rate);
			cout << "FrameNO " << frameNo << "  ���ٶȣ�" << w << "r/min" << endl;

			imshow("dframe1", dframe1);
			imshow("contours", result);

			if (cv::waitKey(delaytime) > 0)
			{
				break;
			}
		}
	}
}