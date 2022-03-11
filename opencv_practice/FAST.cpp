#include "edge_detection.h"
#include <thread>

// FAST algorithm 구현
// image에서 관심이 있는 지점의 픽셀 p를 고른다.
// 이 pixel의 강도(intensity)를 I_p로 설정
// 적합한 임계값 t를 설정
// 테스트 대상 pixel p 주위에 원을 그려 16개의 pixel을 고려함
// pixel p보다 t 이상 밝거나, 어두운 pixel의 개수 n을 구함
// n이 9개 이상이면 p를 corner로 인지 후 원으로 표시


// 1. real time 영상에서 frame 단위로 image를 capture하는 코드(여기서는 5초마다)
// 2. capture한 image에서 각 pixel의 밝기에 따른 값을 알아야 함
// 3. 관심점을 찾아 주변에 원을 그림(모든 점에 대해 검사)
// 4. p와 주변 점들을 비교하는 코드(비교는 decision  tree 방식으로 하는데 그럼 후처리가 필요)
// 5. 인접한 점들 중 극대값을 찾기 위해 후처리로 non-maximal suppression(비최대 억제)를 적용


// real time에서 capture하는 코드는 Video_Capture() 함수를 수정해서 사용
// 처음에는 capture가 아닌 단일 image로 실험
// capture한 image는 640X480(=307200)

// capture image의 p(1)부터 p(307200)까지 검사
// p점에 지름 3인 원을 그려 범위 설정
// 임계값 t 설정(10 정도?)
// 주변 1~16개의 pixel과의 연산
// 큰값 혹은 작은 값이 9개 이상이면 코너점으로 등록

// point p와 주변 점들 사이의 차이를 계산하는 함수
void gap(Mat& src, const int& t, int j, int i, int j_gap, int i_gap, int &n, int &m) {
	if ((static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at <uchar>(j + j_gap, i + i_gap))) > t)
		n += 1;
	else if (abs((static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at <uchar>(j + j_gap, i + i_gap)))) > t)
		m += 1;
}

// Video_Cap 함수를 만들어서 thread로 나누어 작업
// thread 1
void video_cap() {
	FAST_Video_Capture();
	cout << "thread1 finish" << endl;
}

// point p가 선(line)이라면 그 선분의 중간은 제외하는 함수
void line_eliminate(Mat& src, const int& t, int j, int i, int j_gap, int i_gap, int& count) {
	if (abs((static_cast<int>(src.at<uchar>(j - j_gap, i - i_gap)) - static_cast<int>(src.at<uchar>(j + j_gap, i + i_gap)))) < t) {
		count -= 2;
	}
}

// thread 2
int main_work()
{
	cout << "new working start" << endl;

	// Mat src = imread(path_source, IMREAD_GRAYSCALE);
	// 실시간 영상을 캡쳐해서 바로바로 사용하는 코드로 수정

	// 5초마다 영상을 캡쳐
	Mat src = imread("cap.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cout << "Image load failed!" << endl;
		return -1;
	}

	imshow("src", src);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	// 임계값 t
	int t = 70;

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
				int t = 50;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j - 1, i))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j + 1, i))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j, i - 1))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j, i + 1))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j - 1, i - 1))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j + 1, i + 1))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j - 1, i + 1))) > t)) count += 1;
				if ((abs(static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at<uchar>(j + 1, i - 1))) > t)) count += 1;

				line_eliminate(src, t, j, i, 1, 1, count);
				line_eliminate(src, t, j, i, 1, -1, count);
				line_eliminate(src, t, j, i, -1, 1, count);
				line_eliminate(src, t, j, i, -1, -1, count);

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
	waitKey(0);		// esc 키를 누를 때까지 기다림
	destroyAllWindows();


	cout << "thread2 finish" << endl;


	return 0;
}

int FAST()
{
	while (1) {
		thread th1(video_cap);
		thread th2(main_work);

		th1.join();
		th2.join();
	}
	return 0;
}