#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

/// <summary>
/// If the image is equalized per channel, the colors may be distorted.
/// </summary>
/// <param name="img"></param>
/// <param name="dest"></param>
void histoEqRGB(const Mat& img, Mat& dest) {
	vector<Mat> chs;
	split(img, chs);

	//kulon-kulon kiegyenlitjuk a csatornat -> ez okozza a torzulast
	equalizeHist(chs[0], chs[0]);
	equalizeHist(chs[1], chs[1]);
	equalizeHist(chs[2], chs[2]);

	merge(chs, dest);
}

/// <summary>
/// If you convert the image to Lab color space, 
/// you can apply the histogram tranformation on the Luminance channel. 
/// The other two channels, which describes colors, remain unchanged.
/// </summary>
/// <param name="img"></param>
/// <param name="dest"></param>
void histoEqLab(const Mat& img, Mat& dest) {
	Mat tmp;
	cvtColor(img, tmp, COLOR_BGR2Lab);
	
	vector<Mat> chs;
	split(tmp, chs);  //now you have three one channel image (CV_8UC1): L, a, b

	//chs[0] is the L (luminance) channel
	equalizeHist(chs[0], chs[0]);

	merge(chs, tmp);

	cvtColor(tmp, dest, COLOR_Lab2BGR);
}

/// <summary>
/// Similarly, you can use the V channel of the HSV color space. 
/// </summary>
/// <param name="img"></param>
/// <param name="dest"></param>
void histoEqHSV(const Mat& img, Mat& dest) {
	Mat tmp;
	cvtColor(img, tmp, COLOR_BGR2HSV);

	vector<Mat> chs;
	split(tmp, chs);  //now you have three one channel image (CV_8UC1): Hue, Saturation, Value

	//chs2 is the V (Value) channel
	equalizeHist(chs[2], chs[2]);

	merge(chs, tmp);

	cvtColor(tmp, dest, COLOR_HSV2BGR);
}

int main() {
	Mat img = imread("orange1.jpg");
	if (img.empty()) {
		cout << "The file cannot be loaded." << endl;
		exit(-1);
	}

	Mat destRGB;
	histoEqRGB(img, destRGB);

	Mat destLab;
	histoEqLab(img, destLab);

	Mat destHSV;
	histoEqHSV(img, destHSV);

	imshow("original", img);
	imshow("equalized (RGB)", destRGB);
	imshow("equalized (Lab)", destLab);
	imshow("equalized (HSV)", destHSV);

	waitKey(0);

	return 0;
}