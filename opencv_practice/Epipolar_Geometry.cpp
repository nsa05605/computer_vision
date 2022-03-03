#include "edge_detection.h"

void epiploar_geo(string path_source, string path_target)
{
	Mat img1 = imread(path_source, IMREAD_GRAYSCALE);
	Mat img2 = imread(path_target, IMREAD_GRAYSCALE);
	m_resize(img1, img1);
	m_resize(img2, img2);


}