#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace std;
using namespace cv;

void Harris_Corner(Mat img)
{
	Mat src, imgGRAY;																	// src는 입력 image
	cvtColor(img, imgGRAY, COLOR_BGR2GRAY);
	
	if (imgGRAY.rows > 1000)														// size 맞춰주기
	{
		resize(imgGRAY, src, Size(), 0.3, 0.3);
		// Mat src = imread("apple.png", IMREAD_GRAYSCALE);
	}
	else if ((imgGRAY.rows < 100) && (imgGRAY.rows > 300))
	{
		resize(imgGRAY, src, Size(), 3, 3);
	}
	else
	{
		resize(imgGRAY, src, Size(), 1, 1);
	}

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return;
	}

	Mat harris;
	cornerHarris(src, harris, 3, 3, 0.04);

	Mat harris_norm;
	normalize(harris, harris_norm, 0, 255, NORM_MINMAX, CV_8U);					// 0~255 사이의 값으로 정규화

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	for (int j = 1; j < harris.rows - 1; j++) {
		for (int i = 1; i < harris.cols - 1; i++) {
			if (harris_norm.at<uchar>(j, i) > 120) {							// 정규화 코너값이 120을 넘는 것 중
				if (harris.at<float>(j, i) > harris.at<float>(j - 1, i) &&
					harris.at<float>(j, i) > harris.at<float>(j + 1, i) &&
					harris.at<float>(j, i) > harris.at<float>(j, i - 1) &&
					harris.at<float>(j, i) > harris.at<float>(j, i + 1))		// 비최대 억제
				{
					circle(dst, Point(i, j), 5, Scalar(0, 0, 255), 2);			// circle은 (그릴 위치, 좌표, 지름, BGR, 두께)
				}
			}
		}
	}

	imshow("src", src);
	imshow("harris_norm", harris_norm);
	imshow("dst", dst);

	waitKey(0);
	destroyAllWindows();
}