#include "Features.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;




cv::Mat _getGaussianKernel2D(double sigmaX, double sigmaY) {
	int ksize = (int)((int)(2 * max(sigmaX, sigmaY) + 0.5) | 1);
	Mat gx = getGaussianKernel(ksize, sigmaX, CV_32F);
	Mat gy = getGaussianKernel(ksize, sigmaY, CV_32F);
	Mat k = gx.t() * gy;
	return k;
}

inline double gauss2order(double sigma, double x) {
	double num = x*x;
	double variance = sigma*sigma;
	double denom = 2 * variance;
	double g = exp(-num / denom) / sqrt(CV_PI*denom);
	g *= (num - variance) / (variance*variance);
	return g;
}

void _getLaplaceKernel(cv::Mat& kernel, double sigma) {
	int ksize = (int)((int)(2 * sigma + 0.5) | 1);
	int half = ksize / 2;
	kernel.create(Size(ksize, 1), CV_32F);
	for (int i = -half; i <= half; ++i) {
		kernel.at<float>(i + half) = gauss2order(sigma, i);
	}
	double s = cv::sum(cv::abs(kernel))[0];
	kernel *= (1.0 / s);
}

void _getLaplaceKernel2D(cv::Mat& kernel, double sigmaX, double sigmaY) {
	cv::Mat kx, ky;
	_getLaplaceKernel(kx, sigmaX);
	_getLaplaceKernel(ky, sigmaY);
	kernel = (kx.t() * ky);
	//std::cout << kx << ky << kernel << endl;
}

void Features::MaximumResponse(const cv::Mat& img, cv::Mat& dest, double sigmaX, double sigmaY, int gauss) {
	dest.create(img.size(), CV_32FC1);

	int wh = img.cols*img.rows;

	int ksize = (int)((int)(2 * max(sigmaX, sigmaY) + 0.5) | 1);
	cv::Mat tmp;

	double gamma = sigmaX / sigmaY;

	double angleStep = CV_PI / 6.0;

	cv::Mat max = dest;

	for (int i = 0; i < 6; ++i) {
		float theta = i*angleStep;

		cv::Mat kernel;
		if (gauss)
			kernel = _getGaussianKernel2D(sigmaX, sigmaY);
		else
			_getLaplaceKernel2D(kernel, sigmaX, sigmaY);

		cv::filter2D(img, tmp, CV_32F, kernel);

		if (!i) {
			tmp.copyTo(max);
		}
		else {
			float* tf = (float*)tmp.data;
			float* td = (float*)max.data;

			for (int i = 0; i < wh; ++i)
				if (tf[i] < td[i])
					td[i] = tf[i];
		}

		kernel.release();
	}
}



void Features::MaximumResponse8(const cv::Mat& img, vector<cv::Mat>& dest) {
	dest.clear();

	float sigmax[] = { 1.0, 2.0, 4.0 };
	float sigmay[] = { 3.0, 6.0, 12.0 };

	cv::Mat imgf;
	img.convertTo(imgf, CV_32F, 1 / 255.0);

	//dest.create(img.size(), CV_32FC(8));
	cv::Mat tmp;
	for (int i = 0; i < 3; ++i) { //gauss elforgatottak
		MaximumResponse(imgf, tmp, sigmax[i], sigmay[i], 1);
		dest.push_back(tmp.clone());
	}
	for (int i = 0; i < 3; ++i) { //laplace elforgatottak
		MaximumResponse(imgf, tmp, sigmax[i], sigmay[i], 0);
		dest.push_back(tmp.clone());
	}

	// gauss
	cv::GaussianBlur(imgf, tmp, cv::Size(21, 21), 10.0, 10.0);
	dest.push_back(tmp.clone());

	// laplace
	img.convertTo(tmp, CV_32F);
	cv::Mat kernel;
	_getLaplaceKernel2D(kernel, 10.0, 10.0);

	cv::filter2D(imgf, tmp, CV_32F, kernel);
	dest.push_back(imgf.clone());
}


void Features::writeCsv(std::string path, const cv::Mat mat, bool append) {
	int mode = std::ofstream::out | (append ? std::ofstream::app : 0);
	ofstream outputFile(path, mode);
	outputFile << format(mat, cv::Formatter::FMT_CSV) << endl;
	outputFile.close();
}
