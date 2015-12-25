#include"common.h"

void drawDetectLines(Mat& image, const vector<Vec4i>& lines, Scalar & color)
{
	// 将检测到的直线在图上画出来 
	vector<Vec4i>::const_iterator it = lines.begin();
	while (it != lines.end())
	{
		Point pt1((*it)[0], (*it)[1]);
		Point pt2((*it)[2], (*it)[3]);
		line(image, pt1, pt2, color, 2); //  线条宽度设置为2 
		++it;
	}
}



