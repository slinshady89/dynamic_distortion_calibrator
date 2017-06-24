#include "opencv2\opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argv, char** argc) {

	Mat src = imread("column_pattern_10.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dst,edg;
		
	Mat map_x_1, map_y_1;
	map_x_1.create(src.size(), CV_32FC1);
	map_y_1.create(src.size(), CV_32FC1);
	float k1 = 0.0001, k2 = 0.0002,p1=-0.00001, p2=-0.00002;

	for (int y = 0; y<src.rows; y++) {
		for (int x = 0; x<src.cols; x++) {
			int r = (y ^ 2) + (x ^ 2);
			map_x_1.at<float>(y, x) = (1 + k1*r + k2*(r ^ 2))*x+ (2 * p1*x*y + p2*(r + 2 * x ^ 2));
			map_y_1.at<float>(y, x) = (1 + k1*r + k2*(r ^ 2))*y+ (2 * p2*x*y + p1*(r + 2 * y ^ 2));
			
		}
	}

	remap(src, dst, map_x_1, map_y_1, CV_INTER_LINEAR);
	GaussianBlur(dst, dst, Size(3,3), 0,0);

	imshow("original", src);
	imshow("distorted", dst);

	Canny(dst, edg, 150, 360);
	namedWindow("Canny", CV_WINDOW_FULLSCREEN);
	imshow("Canny", edg);
	waitKey();
}
