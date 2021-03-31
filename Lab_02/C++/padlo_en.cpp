#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


void convert(const Mat rgb, vector<Mat>& dest) {
	//we normalize the RGB image: 
	//rgb: 8bit/channel: R, G, B values in [0; 255]
	//rgbf: 32bit/channel: r, g, b values in [0.0; 1.0]  (floats)
	Mat rgbf;
	rgb.convertTo(rgbf, CV_32FC3, 1 / 255.0);

	//we change the color space, because the Lab is more appropriate to measure color difference
	Mat labf;
	cvtColor(rgbf, labf, COLOR_BGR2Lab);

	//we split the image. 
	//labf is a 3-channel image after we split it, we have three one-channel image in a dest vector. 
	cv::split(labf, dest);
}

int main() {
	//we read the benchmark image (add the directory to the path, if you need)
	Mat benchmark = imread("Oranges.jpg");
	//we will get an error message, if the image does not exist
	assert(!benchmark.empty());

	//we convert it with our function
	vector<Mat> benchmark_channels;
	convert(benchmark, benchmark_channels);   //echannels[0], echannels[1] and echannels[2] contains the L, a, b image respectively

	//we will read the other image one by one
	for (int i = 1; i <= 7; ++i) {
		//we read a sample image (add the directory to the path, if you need)
		string path = "floor_tile" + std::to_string(i) + ".png";
		Mat sample = imread(path);
		//we will get an error message, if the image does not exist
		assert(!sample.empty());

		//we convert it with our function
		vector<Mat> sample_channels;
		convert(sample, sample_channels); //channels[0], channels[1] and channels[2] contains the L, a, b image respectively

		//the total color difference is E = sqrt((L1-L2)^2+(a1-a2)^2+(b1-b2)^2. 
		//first we compute the differences                                           
		Mat dL = sample_channels[0] - benchmark_channels[0];   //L1-L2
		Mat da = sample_channels[1] - benchmark_channels[1];   //a1-a2
		Mat db = sample_channels[2] - benchmark_channels[2];   //b1-b2

		//we compute the E = sqrt(dL^2+da^2+db^2). 
		//mul and sqrt is an element wise operation. The result is stored in E.  
		//Each pixel of E is an total color difference of a benchmark pixel and a sample pixel.  
		Mat E;
		cv::sqrt(dL.mul(dL) + da.mul(da) + db.mul(db), E);
		//Find out the average total color difference of the images
		Scalar avg = mean(E);

		//imshow("rgb", rgb);
		//waitKey(0);

		//print out the result
		std::cout << i << ": ";
		if (avg[0] <= 1.0)
			std::cout << "Not perceptible by human eyes." << endl;
		else if (0.1 < avg[0] && avg[0] <= 2.0)
			std::cout << "Perceptible through close observation." << endl;
		else if (2.0 < avg[0])
			std::cout << "Percebtible at a glance." << endl;
		
	}

	system("pause");

	return 0;
}