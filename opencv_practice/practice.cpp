#include "edge_detection.h"

// point p�� �ֺ� ���� ������ ���̸� ����ϴ� �Լ�
void gap(Mat& src, const int& t, int j, int i, int j_gap, int i_gap, int& n, int& m) {
	if ((static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at <uchar>(j + j_gap, i + i_gap))) > t)
		n += 1;
	else if (abs((static_cast<int>(src.at<uchar>(j, i)) - static_cast<int>(src.at <uchar>(j + j_gap, i + i_gap)))) > t)
		m += 1;
}

// point p�� ��(line)�̶�� �� ������ �߰��� �����ϴ� �Լ�
void line_eliminate(Mat& src, const int& t, int j, int i, int j_gap, int i_gap, int &count) {
	if (abs((static_cast<int>(src.at<uchar>(j - j_gap, i - i_gap)) - static_cast<int>(src.at<uchar>(j + j_gap, i + i_gap)))) < t) {
		count -= 2;
	}
}

int main()
{
	// cout << "new working start" << endl;

	// Mat src = imread(path_source, IMREAD_GRAYSCALE);
	// �ǽð� ������ ĸ���ؼ� �ٷιٷ� ����ϴ� �ڵ�� ����
	// FAST_Video_Capture();
	// 5�ʸ��� ������ ĸ��
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

	// �Ӱ谪 t
	int t = 70;

	for (int j = 164; j < 173; ++j) {
		for (int i = 87; i < 91; ++i) {
			cout << '[' << j << "," << i << ']' << static_cast<int>(src.at<uchar>(j, i)) << " ";
		}
		cout << endl;
	}
	/*
	
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
	waitKey(0);		// esc Ű�� ���� ������ ��ٸ�

	cout << "thread2 finish" << endl;

	*/
	return 0;
}



// ���ӵǴ� �� ���� x
//   -> ������ �ƴ� ������ �����ϸ� �Ǵ� �����̹Ƿ�, ���� ��� �Ӱ谪���� ���̰� ������ �����ϵ��� ��