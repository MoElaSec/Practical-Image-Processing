#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


int main() {
	Mat img1 = imread("dragonfly.png", IMREAD_GRAYSCALE);
	Mat img2 = imread("depth.png", IMREAD_GRAYSCALE);

	if (img1.empty() || img2.empty()) {
		cout << "Image cannot be loaded." << endl;
		exit(-1);
	}

	namedWindow("filter", WINDOW_NORMAL);
	resizeWindow("filter", 500, 100);


	int radius = 1;
	createTrackbar("sugar", "filter", &radius, 50);

	int sigma = 10;
	createTrackbar("sigma", "filter", &sigma, 50); 

	namedWindow("gauss", WINDOW_NORMAL);

	int q, ksize = 1;
	Mat b1, mb1, gb1;
	Mat b2, mb2, gb2;

	Mat gk;

	int first = 1;
	while ((q = waitKey(40)) != 'q') {
		ksize = max(2 * radius + 1, 3);
		sigma = max(sigma, 10);

		blur(img1, b1, Size(ksize, ksize));
		blur(img2, b2, Size(ksize, ksize));

		medianBlur(img1, mb1, ksize);
		medianBlur(img2, mb2, ksize);

		GaussianBlur(img1, gb1, Size(ksize, ksize), sigma/10.0);
		GaussianBlur(img2, gb2, Size(ksize, ksize), sigma/10.0);

		gk = cv::getGaussianKernel(ksize, sigma / 10.0);
		gk *= gk.t();
		imshow("gauss", gk);

		imshow("blur 1", b1);
		imshow("blur 2", b2);
		imshow("median filter 1", mb1);
		imshow("median filter 2", mb2);
		imshow("gaussian filter 1", gb1);
		imshow("gaussian filter 2", gb2);

	}

	waitKey(0);

	return 0;
}