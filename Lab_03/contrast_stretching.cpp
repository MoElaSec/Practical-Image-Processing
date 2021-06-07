#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void createHisto256(const Mat& img, Mat& histo) {
	assert(img.type() == CV_8UC1);

	histo = Mat::zeros(256, 1, CV_32S); //or CV_16U

	for (int i = 0; i < img.rows; ++i) {
		for (int j = 0; j < img.cols; ++j) {
			histo.at<int>(img.at<uchar>(i, j))++;
		}
	}
}

void createHistoImg(const Mat& histo, Mat& dest) {
	int padding = 10;
	Size canvas_size(histo.rows, 100);
	
	dest = Mat::zeros(canvas_size, CV_8UC3);
	
	double minVal;
	double maxVal;
	minMaxLoc(histo, &minVal, &maxVal);
	double s = canvas_size.height / maxVal;

	Point p1(0, 0), p2(0, 0);
	Scalar color(0, 255, 0);

	for (int i = 0; i < histo.rows; ++i) {
		p1.x = p2.x = i;
		p2.y = histo.at<int>(i) * s;
		line(dest, p1, p2, color, 1);
	}

	flip(dest, dest, 0);
	copyMakeBorder(dest, dest, 10, 10, 10, 10, BORDER_CONSTANT, 0);
}

void displayHisto(const Mat& img, const string& title) {
	Mat histo, canvas;
	createHisto256(img, histo);
	createHistoImg(histo, canvas);
	imshow(title, canvas);
}


void contrastStretching(const Mat& img, Mat& dest, int minv, int maxv) {
	dest = maxv == minv ? img.clone() : (img - minv) * 255.0 / (maxv - minv);
}

void histoStretching(const Mat& img, Mat& dest) {
	double minv, maxv;
	minMaxLoc(img, &minv, &maxv);
	contrastStretching(img, dest, minv, maxv);
}

int main() {

	Mat img = imread("dark_img.jpg", IMREAD_GRAYSCALE);
	if (img.empty()) {
		cout << "Image cannot be loaded." << endl;
		exit(-1);
	}
	
	Mat res1;
	histoStretching(img, res1);

	//set the parameter according to the histogram of the image
	Mat res2;
	double minv, maxv;
	minMaxLoc(img, &minv, &maxv);
	//here we sacrificed the visibility of high intensities in favor of the low ones (darker area).
	contrastStretching(img, res2, minv, maxv * 0.4);

	imshow("original", img);
	imshow("after histogram stretching", res1);
	imshow("after contrast stretching", res2);

	displayHisto(img, "hist: original");
	displayHisto(res1, "hist: histo stretching");
	displayHisto(res2, "hist: contrast stretching");
	waitKey(0);

	return 0;
}