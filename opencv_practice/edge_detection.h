#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace std;
using namespace cv;

#pragma once

void Harris_Corner(string path_source);
void Canny_Edge(String path_source);
void extract(const Mat& img, Mat& des, vector<KeyPoint>& kp);
int homography(string path_source, string path_target);
int key_match(string path_source, string path_target);