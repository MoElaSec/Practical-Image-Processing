#ifndef  __FEATURES_H__
#define  __FEATURES_H__

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>

namespace Features
{

	void MaximumResponse(const cv::Mat& img, cv::Mat& dest, double sigmaX, double sigmaY, int gauss = 1);

	void MaximumResponse8(const cv::Mat& img, std::vector<cv::Mat>& dest);

	void writeCsv(std::string, const cv::Mat, bool mode = false);

};

#endif // ! __FEATURES_H__
