#define _CRT_SECURE_NO_WARNINGS
#include "edge_detection.h"

int Video_Capture()
{
	int index = 0;
	char buf[256];
	int keycode = 'n';
	Mat frameMat;

	VideoCapture cap(0);
	// VideoCapture '변수 이름'('카메라 번호')

	if (!cap.isOpened())
	{
		printf("can not open \n");
	}
	// namedWindow("image", WINDOW_AUTOSIZE);

	while (1)
	{
		cap.read(frameMat);

		imshow("img", frameMat);

		sprintf(buf, "C:/Users/nsa05/source/repos/opencv_practice/opencv_practice/img_%06d.jpg", index);
		// 저장 위치 및 이름
		cout << buf << endl;
		imwrite(buf, frameMat);
		index++;
		if (index == 999999) index = 0;

		Sleep(2000);
		// 2초 대기

		if (waitKey(25) == 27) break;
	}
	
	return 0;
}