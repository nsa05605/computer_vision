#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
#include "edge_detection.h"

using namespace std;
using namespace cv;

int main()
{
	string path = "building.png";
	Mat img = imread(path);

	char in;
	cin >> in;

	if (in == 'c')
	{
		// Canny Edge
		Canny_Edge(img);
	}
	else if (in == 'h')
	{
		// Harris Corner
		Harris_Corner(img);
	}

	return 0;
}