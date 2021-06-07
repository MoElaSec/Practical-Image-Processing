/**
* Compute threshold value using the foreground pixles/all_pixels formula.
* Use it as a first step, if you have to process e.g. many test sheets, similar documents, etc. 
* Find the ratio on an image manually, and use that for the other ones too. 
*/

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

///
/// A function to create a histogram of a 8-bit grayscale image. (CV_8UC1)
/// img: 8-bit grayscale image
/// hist: a matrix (can be empty) to store the result
///
void getHisto(const cv::Mat img, cv::Mat& histo) {
	assert(img.type() == CV_8UC1);   //check the input

	vector<Mat> images;
	images.push_back(img); // put the image (or images)

	vector<int> channels;
	channels.push_back(0); // we use the 0th channel of this image (actually there are no other channels, because the image contains only 1 channel.)

	vector<int> histo_sizes;
	histo_sizes.push_back(256);  //the number of the different intesity values

	vector<float> histo_ranges;
	histo_ranges.push_back(0.0f); //the lowest intensity value (pure black)
	histo_ranges.push_back(255);  //the highest intenisty value (pure white)
										
	calcHist(images, channels, noArray(), histo, histo_sizes, histo_ranges, false);
}


///
/// A function to threshold of a 8-bit grayscale image. (CV_8UC1)
/// img: 8-bit grayscale image
/// dest: a matrix (can be empty) to store the result
/// ratio: the required ratio of the foreground pixels and the all pixels of the image. 
/// Note: it is for document segmentation so the dark pixels (the text) considered as foreground pixels. 
///       In the result image the text pixels will be white.
///
void doc_threshold(const cv::Mat img, Mat& dest, double ratio = 0.1) {
	assert(img.type() == CV_8UC1);   //check the input

	Mat histo;
	getHisto(img, histo);

	double requredNumberofForegroundPixels = img.cols * img.rows * ratio; 
	
	double s = 0;
	double th = 255;
	for (int i = 0; i < histo.rows; ++i) {
		s += histo.at<float>(i);
		if (s > requredNumberofForegroundPixels) {
			th = i;   //save the threshold value
			break;
		}
	}
	threshold(img, dest, th, 255, THRESH_BINARY_INV);  //do the threshold
}


int main() {

	Mat img = imread("../../scanned3.png", IMREAD_GRAYSCALE);

	if (img.empty()) {
		cout << "the image does not exist" << endl;
		system("pause");
		exit(-1);
	}

	cv::Mat dest;
	doc_threshold(img, dest, 0.1);

	imshow("thresholded", dest);
	waitKey();

	return 0;
}
