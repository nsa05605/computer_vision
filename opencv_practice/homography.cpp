#include "edge_detection.h"

void extract(const Mat& img, Mat& des, vector<KeyPoint>& kp)
{
	const static auto& orb = ORB::create();
	// const static Ptr<ORB> orb = ORB::create();
	// auto는 Ptr로 정해짐
	orb->detectAndCompute(img, noArray(), kp, des);
	// detectAndCompute(image, mask, CV_OUT std::vector<KeyPoint>& keypoints, decriptors, useProvidedKeypoints)
	// image : 입력 이미지
	// mask(optional) : 특징점 검출에 사용할 마스크
	// keypoint : desciptor 계산을 위해 사용할 keypoint
	// descriptors(optional) : 계산된 descriptor
	// useProvidedKeypoints(optional) : True인 경우 특징점 검출을 수행하지 않음
}

int homography(string path_source, string path_target)
{
	Mat first_image = imread(path_source, IMREAD_GRAYSCALE);
	// 이미지 읽기
	// Mat first_image = imread("Source.png", IMREAD_GRAYSCALE);
	Mat second_image = imread("Target.png", IMREAD_GRAYSCALE);
	// decriptor 행렬
	Mat des_first, des_second;
	// keypoint 벡터
	vector<KeyPoint> kp_first, kp_second;
	std::vector<vector<DMatch>> matches;
	// 2차원 vector
	// 1차원 vector DMatch가 들어있는 vector matches
	// DMatch에는 distance, imgidx, queryidx, trainidx 4개의 요소가 들어가 있음

	extract(first_image, des_first, kp_first);
	// cout << "keypoint1.size() = " << kp_first.size() << endl;		// 474, img1 keypoint 개수 출력?
	// cout << "descriptor1.size() = " << des_first.size() << endl;		// [32 x 474]
	extract(second_image, des_second, kp_second);
	// cout << "keypoint2.size() = " << kp_second.size() << endl;		// 500, img2 keypoint 개수 출력?
	// cout << "descriptor2.size() = " << des_second.size() << endl;	// [32 x 500]
	Ptr<DescriptorMatcher> Match_ORB = BFMatcher::create(NORM_HAMMING);
	// 해밍거리 사용
	// BFMatcher는 전수 조사(Brute-Force) 매칭을 수행함
	// query와 train 집합에 있는 모든 descriptor 사이의 거리(차이)를 계산하고,
	// 이중 작은 descriptor를 찾아 매칭함
	// 직관적인 방법이지만, keypoint 개수가 늘어날수록 계산횟수가 급격히 늘어날 수 있다.


	Match_ORB->knnMatch(des_first, des_second, matches, 2);
	// knnMatch는 비슷한 descriptor k개를 매칭하는데 여기서 k=2
	// 여기에서 matches vector [0],[1]에 값이 할당됨

	const int match_size = matches.size();
	sort(matches.begin(), matches.end());
	// sort함수는 기본적으로 sort(start, end)를 이용하면 인자를 오름차순으로 정렬함
	// matches[0][0].distance를 기준으로 오름차순으로 정렬이 됨

	// cout << matches.size() << endl;		// 474쌍 -> (0,0)부터 (473, 1)까지?
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
		// size_t는 시스템에서 포함할 수 있는 최대 크기의 데이터를 표현하는 타입
	{
		if (matches[i][0].distance < ratio_thresh * matches[i][1].distance)
			// 여기서 distance(거리)는 차이를 의미 -> 매칭의 척도
		{
			good_matches.push_back(matches[i][0]);
			// cout << matches[i][0].distance << endl;
		}
	}

	// cout << good_matches.size() << endl;		// 89개
	// cout << "good " << good_matches[88].distance << endl;

	cv::Mat Result;

	cv::drawMatches(first_image, kp_first, second_image, kp_second, good_matches, Result, cv::Scalar::all(-1),
		cv::Scalar(-1), vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	// 매칭점을 이미지에 표시하는 함수
	// first_image, kp_first : 기준 영상과 기준 영상에서 추출한 keypoint
	// second_image, kp_second : 대상 영상과 대상 영상에서 추출한 keypoint
	// good_matches : 매칭 정보(cv2.DMatch의 리스트)
	// Result : 출력 영상
	// cv::Scalar::all(-1) : matchColor, 매칭된 keypoint와 직선 색상, 랜덤한 색상
	// cv::Scalar(-1) : singlePointColor : 매칭되지 않은 keypoint 색상
	// vector<char>() : matchesMask, 매칭 정보를 선택하여 그릴 떄 사용할 마스크
	// cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS : flags, 매칭 정보 그리기 방법
	imshow("result", Result);

	// homography 변환행렬 H 구하기

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