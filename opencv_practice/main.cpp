#include "edge_detection.h"

using namespace std;
using namespace cv;

int main()
{
	string path_source;
	string path_target;
	cout << "input path_source : ";
	cin >> path_source;
	char in;
	cin >> in;

	if (in == 'c') {
		// Canny Edge
		Canny_Edge(path_source);
	}
	else if (in == 'h') {
		// Harris Corner
		Harris_Corner(path_source);
	}
	else if (in == 'H' || in == 'k') {
		cout << "input path_target : ";
		cin >> path_target;
		if (in == 'H') {
			// Homography
			homography(path_source, path_target);
		}
		else if (in == 'k') {
			// Keypoint Matching
			key_match(path_source, path_target);
		}
	}
	return 0;
}