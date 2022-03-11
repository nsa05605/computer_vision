#include "edge_detection.h"
#include <thread>

// FAST algorithm ����
// image���� ������ �ִ� ������ �ȼ� p�� ����.
// �� pixel�� ����(intensity)�� I_p�� ����
// ������ �Ӱ谪 t�� ����
// �׽�Ʈ ��� pixel p ������ ���� �׷� 16���� pixel�� �����
// pixel p���� t �̻� ��ų�, ��ο� pixel�� ���� n�� ����
// n�� 9�� �̻��̸� p�� corner�� ���� �� ������ ǥ��


// 1. real time ���󿡼� frame ������ image�� capture�ϴ� �ڵ�(���⼭�� 5�ʸ���)
// 2. capture�� image���� �� pixel�� ��⿡ ���� ���� �˾ƾ� ��
// 3. �������� ã�� �ֺ��� ���� �׸�(��� ���� ���� �˻�)
// 4. p�� �ֺ� ������ ���ϴ� �ڵ�(�񱳴� decision  tree ������� �ϴµ� �׷� ��ó���� �ʿ�)
// 5. ������ ���� �� �ش밪�� ã�� ���� ��ó���� non-maximal suppression(���ִ� ����)�� ����


// real time���� capture�ϴ� �ڵ�� Video_Capture() �Լ��� �����ؼ� ���
// ó������ capture�� �ƴ� ���� image�� ����
// capture�� image�� 640X480(=307200)

// capture image�� p(1)���� p(307200)���� �˻�
// p���� ���� 3�� ���� �׷� ���� ����
// �Ӱ谪 t ����(10 ����?)
// �ֺ� 1~16���� pixel���� ����
// ū�� Ȥ�� ���� ���� 9�� �̻��̸� �ڳ������� ���

// point p�� �ֺ� ���� ������ ���̸� ����ϴ� �Լ�
void gap(Mat& src, const int& t, int j, int i, int j_gap, int i_gap, int &n, int &m) {
	if ((static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at <uchar>(j + j_gap, i + i_gap))) > t)
		n += 1;
	else if (abs((static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at <uchar>(j + j_gap, i + i_gap)))) > t)
		m += 1;
}

// Video_Cap �Լ��� ���� thread�� ������ �۾�
// thread 1
void video_cap() {
	FAST_Video_Capture();
	cout << "thread1 finish" << endl;
}

// point p�� ��(line)�̶�� �� ������ �߰��� �����ϴ� �Լ�
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
	// �ǽð� ������ ĸ���ؼ� �ٷιٷ� ����ϴ� �ڵ�� ����

	// 5�ʸ��� ������ ĸ��
	Mat src = imread("cap.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cout << "Image load failed!" << endl;
		return -1;
	}

	imshow("src", src);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	// �Ӱ谪 t
	int t = 70;

	//�� pixel ���� ����ϴ� �Լ�
	//col�� ����, row�� ����
	for (int j = 3; j < src.rows - 3; j++) {
		for (int i = 3; i < src.cols - 3; i++) {
			int n = 0, m = 0;
			// �Ʒ� �ĵ��� �Լ��� �ܼ�ȭ
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

			// ���� 3�� ���� ��ġ�� �ȼ������� ���̰� t �̻��� ���� 9�� �̻��̸�(FAST-9)
			if ((n >= 9) || (m >= 9)) {
				// ���ִ� ����
				// �ֺ� 8���� �� �߿� ���̰� t ������ ���� ����� ��
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
	waitKey(0);		// esc Ű�� ���� ������ ��ٸ�
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