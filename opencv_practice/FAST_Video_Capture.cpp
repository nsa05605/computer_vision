#define _CRT_SECURE_NO_WARNINGS
#include "edge_detection.h"

int FAST_Video_Capture()
{
	char buf[256];
	int keycode = 'n';
	Mat frameMat;

	VideoCapture cap(0, CAP_DSHOW);
	// VideoCapture '변수 이름'('카메라 번호')

	if (!cap.isOpened())
	{
		printf("can not open \n");
	}
	// namedWindow("image", WINDOW_AUTOSIZE);

	cap.read(frameMat);

	// imshow("img", frameMat);

	sprintf(buf, "C:/Users/nsa05/source/repos/opencv_practice/opencv_practice/cap.jpg");

	// 저장 위치 및 이름

	cout << buf << endl;
	imwrite(buf, frameMat);

	// Sleep(5000);
	
	return 0;
}