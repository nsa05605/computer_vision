#include "edge_detection.h"

int main()
{
	string path_source;
	string path_target;
	string real_time;

	char in;
	cout << "input name of work" << '\n' << "Canny_Edge : c" << '\n' << "Harris_Corner : h"
		<< '\n' << "Keypoint_Matching : k" << '\n' << "homography : (large) H" << '\n'
		<< "Video_Capture : v" << '\n' << "Pedestrian_Detection : p" << '\n'
		<< "FATS_algorithm : f" << endl;
	cin >> in;

	if (in == 'c' || in == 'h' || in == 'k' || in == 'H' || in == 'f') {
		real_time = "no";
	}
	else if (in == 'v' || in == 'p') {
		real_time = "real";
	}

	if (real_time == "real") {
		if (in == 'v') {
			// VideoCapture
			Video_Capture(5000);
		}
		else if (in == 'p') {
			// pedestrian_detection
			pedestrian_detect();
		}
	}
	else if (real_time == "no") {
		if (in == 'f') {
			// FAST algorithm
			FAST();
		}
		else {
			cout << "input path_source : ";
			cin >> path_source;

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
		}
	}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	return 0;

}