#include "MethodBase.h"


MethodBase::MethodBase()
{
}


MethodBase::~MethodBase()
{
}

bool MethodBase::initVideo(string videoFile)
{
	capture.open(videoFile);
	if (!capture.isOpened())
	{
		std::cout << "read video failure" << std::endl;
		return false;
	}

	 rate = capture.get(CV_CAP_PROP_FPS);
	 delaytime = 1000 / rate;
	 frameNo = 0;

	return true;
}


Mat MethodBase::cluster(Mat inPut, Mat *plabels, int HPthreshold, int clusterCount)
{
#pragma region clusters 
	//Mat labels;
	Mat img(1000, 1000, CV_8UC3);

	vector<Vec2f> lines;
	HoughLines(inPut, lines, 1, 3.14159265 / 180, HPthreshold); //		//	 rho = lines[i][0] 	 theta = lines[i][1];

	img.setTo(0);
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i][0] = sin(2 * lines[i][1]) * 400; //角度换为圆形，用来聚类，避免角度在过零点时的跳变
		lines[i][1] = cos(2 * lines[i][1]) * 400;
		Point ipt;
		ipt.x = lines[i][0] + 500;
		ipt.y = lines[i][1] + 500;
		circle(img, ipt, 2, Scalar(255, 0, 0), CV_FILLED, CV_AA);
	}

	Mat points(lines.size(), 1, CV_32FC2);

	points = Mat(lines);
	Mat centers(clusterCount, 1, points.type());    //用来存储聚类后的中心点



	kmeans(points, clusterCount, *plabels,
		TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0),
		3, KMEANS_PP_CENTERS, centers);  //聚类3次，取结果最好的那次，聚类的初始化采用PP特定的随机算法。
	//cout << "h" << endl;

	for (int i = 0; i < clusterCount; i++)
	{
		Point2f ct = centers.at<Point2f>(i);
		Point ipt;
		ipt.x = ct.x + 500;
		ipt.y = ct.y + 500;
		circle(img, ipt, 2, Scalar(0, 0, 255), CV_FILLED, CV_AA);
	}
	imshow("clusters", img);

	return centers;
#pragma endregion
}

//====================================================================================================
W_calculator::W_calculator()
{
	CurrentDis = Mat(3, 1, CV_32FC2);
	Lastis = Mat(3, 1, CV_32FC2);
	w = 0;
	w_last = 0;
}

int W_calculator::disMarch(Mat CurrentTheata, Mat LastTheata, int len, int IndexTemp[])
{
	int dis = 0;
	for (int i = 0; i < len; i++)
	{
		Point2f A = CurrentTheata.at<Point2f>(i);
		Point2f B = LastTheata.at<Point2f>(IndexTemp[i]);
		dis += (A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y);
	}
	return dis;
}

Mat W_calculator::AdjustCurrentTheata(Mat CurrentTheata, Mat LastTheata, int len)
{
	Point2f A;
	Point2f B;
	float dis;
	float disMin;
	int IndexMin;


	Mat TempTheata(3, 1, CV_32FC2);

	int IndexTemp[6][3] = { { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 }, { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 } };

	disMin = -1;
	for (int i = 0; i < 6; i++)
	{
		dis = disMarch(CurrentTheata, LastTheata, len, IndexTemp[i]);
		if (disMin == -1 || dis < disMin)
		{
			disMin = dis;
			IndexMin = i;
		}

	}

	for (int k = 0; k < len; k++)
	{
		TempTheata.at<Point2f>(k) = CurrentTheata.at<Point2f>(IndexTemp[IndexMin][k]);
	}
	return TempTheata;
}

float W_calculator::calc(Mat CurrentDis, float frameRate)
{
	Mat a = AdjustCurrentTheata(CurrentDis, Lastis, 3);
	Lastis = a;

	for (int i = 0; i < 3; i++)
	{
		float x = a.at<Point2f>(i).x;
		float y = a.at<Point2f>(i).y;
		Theata1[i] = cvFastArctan(y, x) / 2;
	}

	float w;
	w = 0;
	for (int i = 0; i < 3; i++)
	{
		dTheata[i] = Theata1[i] - Theata2[i];
		Theata2[i] = Theata1[i];
		w += dTheata[i];
	}
	w /= 3;
	w = w * frameRate * 60 / 360;

	if (abs(w)>30)w = w_last;

	w = 0.9*w_last + 0.1*w;
	w_last = w;

	return w;
}

