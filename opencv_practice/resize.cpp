#include "edge_detection.h"

void m_resize(Mat &src_bf, Mat &src_af)
{
	
	if (src_bf.rows > 2000)												// size ¸ÂÃçÁÖ±â
	{
		while (src_bf.rows > 800) {
			// resize(src_bf, src_bf, Size(), 0.3, 0.3);
			// Mat src = imread("apple.png", IMREAD_GRAYSCALE);
			resize(src_bf, src_bf, Size(src_bf.cols * 0.5, src_bf.rows * 0.5), 0, 0);
			if (src_bf.rows < 800) {
				resize(src_bf, src_af, Size(src_bf.cols, src_bf.rows), 0, 0);
				break;
			}
		}
	}
	else if ((src_bf.rows < 200))
	{
		resize(src_bf, src_af, Size(src_bf.cols * 3, src_bf.rows * 3), 0, 0);
	}
	else
	{
		resize(src_bf, src_af, Size(src_bf.cols, src_bf.rows), 0, 0);
	}
	

}