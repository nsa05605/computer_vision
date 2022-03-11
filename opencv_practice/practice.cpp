#include "edge_detection.h"

// point p와 주변 점들 사이의 차이를 계산하는 함수
void gap(Mat& src, const int& t, int j, int i, int j_gap, int i_gap, int& n, int& m) {
	if ((static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at <uchar>(j + j_gap, i + i_gap))) > t)
		n += 1;
	else if (abs((static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at <uchar>(j + j_gap, i + i_gap)))) > t)
		m += 1;
}

// point p가 선(line)이라면 그 선분의 중간은 제외하는 함수
void line_eliminate(Mat& src, const int& t, int j, int i, int j_gap, int i_gap, int &count) {
	if (abs((static_cast<int>(src.at<uchar>(j - j_gap, i - i_gap)) - static_cast<int>(src.at<uchar>(j + j_gap, i + i_gap)))) < t) {
		count -= 2;
	}
}

int main()
{
	// cout << "new working start" << endl;

	// Mat src = imread(path_source, IMREAD_GRAYSCALE);
	// 실시간 영상을 캡쳐해서 바로바로 사용하는 코드로 수정
	// FAST_Video_Capture();
	// 5초마다 영상을 캡쳐
	Mat src = imread("board.png", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cout << "Image load failed!" << endl;
		return -1;
	}

	imshow("src", src);

	//for (int j = 446; j < 451; ++j) {
	//	for (int i = 135; i < 140; ++i) {
	//		cout << '[' << j << "," << i << ']' << static_cast<int>(src.at<uchar>(j, i)) << " ";
	//	}
	//	cout << endl;
	//}


	
	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	// 임계값 t
	int t = 70;

	for (int j = 164; j < 173; ++j) {
		for (int i = 87; i < 91; ++i) {
			cout << '[' << j << "," << i << ']' << static_cast<int>(src.at<uchar>(j, i)) << " ";
		}
		cout << endl;
	}
	/*
	
	//각 pixel 값을 출력하는 함수
	//col이 가로, row가 세로
	for (int j = 3; j < src.rows - 3; j++) {
		for (int i = 3; i < src.cols - 3; i++) {
			int n = 0, m = 0;
			// 아래 식들을 함수로 단순화
			gap(src, t, j, i, 3, 0, n, m);
			gap(src, t, j, i, 3, 1, n, m);
			gap(src, t, j, i, 2, 2, n, m);
			gap(src, t, j, i, 1, 3, n, m);
			gap(src, t, j, i, 0, 3, n, m);
			gap(src, t, j, i, -1, 3, n, m);
			gap(src, t, j, i, -2, 2, n, m);
			gap(src, t, j, i, -3, 1, n, m);
			gap(src, t, j, i, -3, 0, n, m);
			gap(src, t, j, i, -3, -1, n, m);
			gap(src, t, j, i, -2, -2, n, m);
			gap(src, t, j, i, -1, -3, n, m);
			gap(src, t, j, i, 0, -3, n, m);
			gap(src, t, j, i, 1, -3, n, m);
			gap(src, t, j, i, 2, -2, n, m);
			gap(src, t, j, i, 3, -1, n, m);

			// 지름 3인 원에 위치한 픽셀값과의 차이가 t 이상인 곳이 9개 이상이면(FAST-9)
			if ((n >= 9) || (m >= 9)) {
				// 비최대 억제
				// 주변 8개의 점 중에 차이가 t 이하인 점이 없어야 함
				// ( ) < t -> false
				int count = 0;
				int t = 60;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j - 1, i))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j + 1, i))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j, i - 1))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j, i + 1))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j - 1, i - 1))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j + 1, i + 1))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j - 1, i + 1))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j + 1, i - 1))) > t)) count += 1;

				line_eliminate(src, t, j, i, 2, 2, count);
				line_eliminate(src, t, j, i, 2, -2, count);
				line_eliminate(src, t, j, i, -2, 2, count);
				line_eliminate(src, t, j, i, -2, -2, count);

				if (count >= 4) {
					cout << '[' << j << "," << i << ']' << static_cast<int>(src.at<uchar>(j, i)) << " ";
					circle(dst, Point(i, j), 5, Scalar(0, 0, 255), 1);
				}
			}
		}
		cout << endl;
	}

	imshow("dst", dst);
	// Sleep(5000);
	// destroyAllWindows();
	waitKey(0);		// esc 키를 누를 때까지 기다림

	cout << "thread2 finish" << endl;

	*/
	return 0;
}



// 연속되는 점 검출 x
//   -> 직선이 아닌 끝점을 검출하면 되는 문제이므로, 양쪽 모두 임계값보다 차이가 적으면 제외하도록 함