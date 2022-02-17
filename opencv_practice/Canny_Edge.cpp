#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace std;
using namespace cv;

void Canny_Edge(Mat img)
{
	//ChangeColor(img);
	//GaussianBlur(img);

	Mat imgGRAY, imgBlur_GRAY_3, imgBlur_GRAY_5;
	Mat imgCanny_GRAY_1, imgCanny_GRAY_2, imgCanny_GRAY_3, imgCanny_GRAY_4;
	Mat imgCanny_GRAY_5, imgCanny_GRAY_6, imgCanny_GRAY_7, imgCanny_GRAY_8;
	// Mat imgHSV, imgBlur_HSV, imgCanny_HSV;


	// size를 절반으로 줄이는 resize
	// RGB를 Gray 혹은 HSV로 전환
	// resize(img, imgResize, Size(), 0.5, 0.5);
	cvtColor(img, imgGRAY, COLOR_BGR2GRAY);
	// cvtColor(img, imgHSV, COLOR_BGR2HSV);

	// imshow("Image", img);
	// imshow("Image Resize", imgResize);
	// imshow("Image Gray", imgGRAY);
	// imshow("Image HSV", imgHSV);


	// Gaussian Blur 적용
	GaussianBlur(imgGRAY, imgBlur_GRAY_3, Size(3, 3), 0);
	// GaussianBlur(imgHSV, imgBlur_HSV, Size(3, 3), 0);
	GaussianBlur(imgGRAY, imgBlur_GRAY_5, Size(7, 7), 0);

	imshow("Gaussian Blur_Gray_3*3", imgBlur_GRAY_3);
	// imshow("Gaussian Blur_HSV", imgBlur_HSV);
	imshow("Gaussian Blur_Gray_5*5", imgBlur_GRAY_5);


	// Canny Edge Detection algorithm 적용
	// 3 X 3 Gaussian에 적용
	Canny(imgBlur_GRAY_3, imgCanny_GRAY_1, 0, 50);
	// Canny(imgBlur_HSV, imgCanny_HSV, 25, 75);
	Canny(imgBlur_GRAY_3, imgCanny_GRAY_2, 25, 75);
	Canny(imgBlur_GRAY_3, imgCanny_GRAY_3, 100, 150);
	Canny(imgBlur_GRAY_3, imgCanny_GRAY_4, 150, 200);

	// 5 X 5 Gaussian에 적용
	Canny(imgBlur_GRAY_5, imgCanny_GRAY_5, 0, 50);
	// Canny(imgBlur_HSV, imgCanny_HSV, 25, 75);
	Canny(imgBlur_GRAY_5, imgCanny_GRAY_6, 25, 75);
	Canny(imgBlur_GRAY_5, imgCanny_GRAY_7, 100, 150);
	Canny(imgBlur_GRAY_5, imgCanny_GRAY_8, 150, 200);

	imshow("Canny_Gray[0,50]_3", imgCanny_GRAY_1);
	// imshow("Canny_HSV", imgCanny_HSV);
	imshow("Canny_Gray[25,75]_3", imgCanny_GRAY_2);
	imshow("Canny_Gray[100,150]_3", imgCanny_GRAY_3);
	imshow("Canny_Gray[150,200]_3", imgCanny_GRAY_4);

	imshow("Canny_Gray[0,50]_5", imgCanny_GRAY_5);
	// imshow("Canny_HSV", imgCanny_HSV);
	imshow("Canny_Gray[25,75]_5", imgCanny_GRAY_6);
	imshow("Canny_Gray[100,150]_5", imgCanny_GRAY_7);
	imshow("Canny_Gray[150,200]_5", imgCanny_GRAY_8);

	waitKey(0);
}