#include"common.h"

void drawDetectLines(Mat& image, const vector<Vec4i>& lines, Scalar & color)
{
	// ����⵽��ֱ����ͼ�ϻ����� 
	vector<Vec4i>::const_iterator it = lines.begin();
	while (it != lines.end())
	{
		Point pt1((*it)[0], (*it)[1]);
		Point pt2((*it)[2], (*it)[3]);
		line(image, pt1, pt2, color, 2); //  �����������Ϊ2 
		++it;
	}
}



