#include "edge_detection.h"

void extract(const Mat& img, Mat& des, vector<KeyPoint>& kp)
{
	const static auto& orb = ORB::create();
	// const static Ptr<ORB> orb = ORB::create();
	// auto�� Ptr�� ������
	orb->detectAndCompute(img, noArray(), kp, des);
	// detectAndCompute(image, mask, CV_OUT std::vector<KeyPoint>& keypoints, decriptors, useProvidedKeypoints)
	// image : �Է� �̹���
	// mask(optional) : Ư¡�� ���⿡ ����� ����ũ
	// keypoint : desciptor ����� ���� ����� keypoint
	// descriptors(optional) : ���� descriptor
	// useProvidedKeypoints(optional) : True�� ��� Ư¡�� ������ �������� ����
}

int homography(string path_source, string path_target)
{
	Mat first_image = imread(path_source, IMREAD_GRAYSCALE);
	// �̹��� �б�
	// Mat first_image = imread("Source.png", IMREAD_GRAYSCALE);
	Mat second_image = imread("Target.png", IMREAD_GRAYSCALE);
	// decriptor ���
	Mat des_first, des_second;
	// keypoint ����
	vector<KeyPoint> kp_first, kp_second;
	std::vector<vector<DMatch>> matches;
	// 2���� vector
	// 1���� vector DMatch�� ����ִ� vector matches
	// DMatch���� distance, imgidx, queryidx, trainidx 4���� ��Ұ� �� ����

	extract(first_image, des_first, kp_first);
	// cout << "keypoint1.size() = " << kp_first.size() << endl;		// 474, img1 keypoint ���� ���?
	// cout << "descriptor1.size() = " << des_first.size() << endl;		// [32 x 474]
	extract(second_image, des_second, kp_second);
	// cout << "keypoint2.size() = " << kp_second.size() << endl;		// 500, img2 keypoint ���� ���?
	// cout << "descriptor2.size() = " << des_second.size() << endl;	// [32 x 500]
	Ptr<DescriptorMatcher> Match_ORB = BFMatcher::create(NORM_HAMMING);
	// �عְŸ� ���
	// BFMatcher�� ���� ����(Brute-Force) ��Ī�� ������
	// query�� train ���տ� �ִ� ��� descriptor ������ �Ÿ�(����)�� ����ϰ�,
	// ���� ���� descriptor�� ã�� ��Ī��
	// �������� ���������, keypoint ������ �þ���� ���Ƚ���� �ް��� �þ �� �ִ�.


	Match_ORB->knnMatch(des_first, des_second, matches, 2);
	// knnMatch�� ����� descriptor k���� ��Ī�ϴµ� ���⼭ k=2
	// ���⿡�� matches vector [0],[1]�� ���� �Ҵ��

	const int match_size = matches.size();
	sort(matches.begin(), matches.end());
	// sort�Լ��� �⺻������ sort(start, end)�� �̿��ϸ� ���ڸ� ������������ ������
	// matches[0][0].distance�� �������� ������������ ������ ��

	// cout << matches.size() << endl;		// 474�� -> (0,0)���� (473, 1)����?
	// cout << "dis " << matches[473][0].distance << endl;

	/*
	for (int i = 0; i < 473; ++i) {
		for (int j = 0; j < 2; ++j) {							// i,j		= 0,0	0,1 	1,0 	1,1 	2,0 	2,1 	3,0
			cout << "dis " << matches[i][j].distance << endl;	// distance = 13	45  	14		33		15		21		16
			cout << "img " << matches[i][j].imgIdx << endl;		// imgidx	= 0		0		0		0		0		0		0
			cout << "que " << matches[i][j].queryIdx << endl;	// queryidx = 269	269		147		147		6		6		268
			cout << "tra " << matches[i][j].trainIdx << endl;	// trainidx = 364	452		289		349		203		321		372
		}
	}
	*/

	vector<DMatch> good_matches;
	const float ratio_thresh = 0.75f;
	for (size_t i = 0; i < matches.size(); i++)
		// size_t�� �ý��ۿ��� ������ �� �ִ� �ִ� ũ���� �����͸� ǥ���ϴ� Ÿ��
	{
		if (matches[i][0].distance < ratio_thresh * matches[i][1].distance)
			// ���⼭ distance(�Ÿ�)�� ���̸� �ǹ� -> ��Ī�� ô��
		{
			good_matches.push_back(matches[i][0]);
			// cout << matches[i][0].distance << endl;
		}
	}

	// cout << good_matches.size() << endl;		// 89��
	// cout << "good " << good_matches[88].distance << endl;

	cv::Mat Result;

	cv::drawMatches(first_image, kp_first, second_image, kp_second, good_matches, Result, cv::Scalar::all(-1),
		cv::Scalar(-1), vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	// ��Ī���� �̹����� ǥ���ϴ� �Լ�
	// first_image, kp_first : ���� ����� ���� ���󿡼� ������ keypoint
	// second_image, kp_second : ��� ����� ��� ���󿡼� ������ keypoint
	// good_matches : ��Ī ����(cv2.DMatch�� ����Ʈ)
	// Result : ��� ����
	// cv::Scalar::all(-1) : matchColor, ��Ī�� keypoint�� ���� ����, ������ ����
	// cv::Scalar(-1) : singlePointColor : ��Ī���� ���� keypoint ����
	// vector<char>() : matchesMask, ��Ī ������ �����Ͽ� �׸� �� ����� ����ũ
	// cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS : flags, ��Ī ���� �׸��� ���
	imshow("result", Result);

	// homography ��ȯ��� H ���ϱ�

	vector<Point2f> first_point;
	vector<Point2f> second_point;

	for (size_t i = 0; i < good_matches.size(); i++) {
		// Get the keypoints from the good_matches
		first_point.push_back(kp_first[good_matches[i].queryIdx].pt);
		second_point.push_back(kp_second[good_matches[i].trainIdx].pt);
	}
	Mat H = findHomography(first_point, second_point, RANSAC);
	cout << "Homography" << endl;
	cout << H << endl;
	while (waitKey(1) != 37) {
		continue;
	}

	return 0;
}