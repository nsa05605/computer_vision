#define _CRT_SECURE_NO_WARNINGS
#include "edge_detection.h"

int FAST_Video_Capture()
{
	char buf[256];
	int keycode = 'n';
	Mat frameMat;

	VideoCapture cap(0, CAP_DSHOW);
	// VideoCapture '���� �̸�'('ī�޶� ��ȣ')

	if (!cap.isOpened())
	{
		printf("can not open \n");
	}
	// namedWindow("image", WINDOW_AUTOSIZE);

	cap.read(frameMat);

	// imshow("img", frameMat);

	sprintf(buf, "C:/Users/nsa05/source/repos/opencv_practice/opencv_practice/cap.jpg");

	// ���� ��ġ �� �̸�

	cout << buf << endl;
	imwrite(buf, frameMat);

	// Sleep(5000);
	
	return 0;
}