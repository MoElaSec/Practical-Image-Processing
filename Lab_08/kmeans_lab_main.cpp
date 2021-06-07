#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace std;
using namespace cv;


int main() {

	Mat img = imread("dog.png");          //any image in color
	imshow("original", img);

	Mat imgf;
	img.convertTo(imgf, CV_32F, 1 / 255.0);  //byte -> float (precise computation)

	Mat lab;
	cvtColor(imgf, lab, COLOR_BGR2Lab);      //we change the color space, because Lab space is more appropriate for measuring the differences of the colors than RGB

	Mat data = lab.reshape(1, img.rows*img.cols);
	
	int K = 2; //number of clusters
	TermCriteria tcrit(TermCriteria::Type::MAX_ITER | TermCriteria::Type::EPS, 100, 0.001);
	
	Mat bestLabels, centers;
	kmeans(data, K, bestLabels, tcrit, 3, KMEANS_RANDOM_CENTERS, centers);

	Mat dest(img.size(), CV_32FC3);

    
	for (int y = 0; y < img.rows; ++y) {    
		for (int x = 0; x < img.cols; ++x) {
			int id = bestLabels.at<int>(y * img.cols + x);  // claster id
			dest.at<Vec3f>(y, x) = Vec3f(centers.at<float>(id, 0), centers.at<float>(id, 1), centers.at<float>(id, 2));  //mean color of the cluster
		}
	}
  
	cvtColor(dest, dest, COLOR_Lab2BGR);  // we go back to the RGB space
	dest.convertTo(dest, CV_8U, 255.0);   // and change back the type

	imshow("eredmeny", dest);
	waitKey();
}