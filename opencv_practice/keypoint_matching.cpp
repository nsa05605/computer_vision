#include "edge_detection.h"

int key_match(string path_source, string path_target)
{
	Mat trg = imread(path_source, IMREAD_GRAYSCALE);
	Mat src = imread(path_target, IMREAD_GRAYSCALE);

	if (trg.empty() || src.empty())
	{	
		cerr << "Image load failed!" << endl;

		return -1;
	}

	Ptr<Feature2D> feature = ORB::create();

	vector<KeyPoint> trgKeypoints, srcKeypoints;
	Mat trgDesc, srcDesc;
	feature->detectAndCompute(trg, Mat(), trgKeypoints, trgDesc);
	feature->detectAndCompute(src, Mat(), srcKeypoints, srcDesc);
	// Detects keypoints and computes the descriptors.

	Ptr<DescriptorMatcher> matcher = BFMatcher::create(NORM_HAMMING);
	// Brute-force matcher create method.

	vector<DMatch> matches;
	// Class for matching keypoint descriptors.
	matcher->match(trgDesc, srcDesc, matches);
	// Finds the best match for each deescriptor from a query set.

	sort(matches.begin(), matches.end());
	vector<DMatch> good_matches(matches.begin(), matches.begin() + 100);

	Mat dst;
	drawMatches(trg, trgKeypoints, src, srcKeypoints, good_matches, dst,
		Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	// Draws the found matches of keypoints from two images.

	imshow("dst", dst);

	waitKey();

	return 0;
}