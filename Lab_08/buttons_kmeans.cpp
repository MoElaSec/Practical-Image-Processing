#include<iostream>   
#include<vector>
#include<opencv2/core.hpp> //data structures, core functions
#include<opencv2/highgui.hpp> //gui
#include<opencv2/imgproc.hpp> //image processing function


using namespace std;
using namespace cv;


int main() {

	//reading the image in grayscale
	Mat img = imread("../kepek/buttons.png", IMREAD_COLOR);

	Mat rgb = img.clone(); //a copy for visualization purpose (contours)
	Mat rgb2 = img.clone(); //a copy for visualization purpose (polygons)

	//convert the image to grayscale
	//because we will use thesholding method in the next step
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	imwrite("b1_gray.png", gray);

	//The threshold value is high (250), almost white, because the background is pure white
	//The foreground objects are darker than the background, so we use THRESH_BINARY_INV
	//We using 255, because we want black and white result (mask of the buttons)
	Mat mask;
	threshold(gray, mask, 250, 255, THRESH_BINARY_INV);
	imwrite("b2_button_mask.png", mask);

	//a little noise filtering (very small, false objects will disappear)
	medianBlur(mask, mask, 7);
	imwrite("b3_filtered_mask.png", mask);

	//One of the descriptor (feature) by which we can distinguish the buttons 
	//can be whether there is a hole in it or not.
	//That's why we'll use hierarchy and RETR_TREE method, to find the relationship between the contours.
	vector<vector<Point>> contours;
	vector<Vec4i> hier;
	findContours(mask.clone(), contours, hier, RETR_TREE, CHAIN_APPROX_NONE);

	//Let's collect the top level objects (buttons without holes)
	vector<int> buttons;
	for (int i = 0; i < hier.size(); ++i) {
		// hier[i][3] is the index of parent contour of the contours vector
		// if the value is -1, the ith contour has no parents => top level contour
		if (hier[i][3] == -1) { 
			buttons.push_back(i); // we save the index of the button

			//drawing the ith contour
			drawContours(rgb, contours, i, Scalar(rand() & 255, rand() & 255, rand() & 255), 2);
		}
	}

	int numObjs = buttons.size();  //the number of the top level objects

	// feature matrix for clastering
	// each button has its own row that contains the descriptors (features) for that button
    // this time we are going to use 4 features, but you can use more features if you want
	Mat data(numObjs, 4, CV_32F);

	Mat hsv;
	cvtColor(img, hsv, COLOR_BGR2HSV);
	for (int i = 0; i < numObjs; ++i) {
		int ctrIdx = buttons[i]; //contours[ctrIdx] - ith top level contour, hier[ctrIdx] the corresponding heirarchy information

        // just an alias for the ith top level contour
		vector<Point>& pts = contours[ctrIdx];

		// the bounding rect of the pts 
		Rect r = boundingRect(pts);

		// for visualization purpose
		rectangle(rgb2, r, Scalar(0, 255, 0), 1); 

		// we approximate the object with a polygon, 
		// because the number of polygon vertices will be one of the features
		// 3 - how much the approximation may deviate from the original contour
		vector<Point> approx;
		approxPolyDP(pts, approx, 3, true);

		// for visualization purpose
		polylines(rgb2, approx, true, Scalar(0, 0, 255), 2);

		// fill the data (features) matrix
		data.at<float>(i, 0) = mean(hsv(r), mask(r))[0];
		data.at<float>(i, 1) = hier[ctrIdx][2] == -1;
		data.at<float>(i, 2) = contourArea(pts);
		data.at<float>(i, 4) = approx.size();
	}

	// the area of the objects will determine everything
	// the other values are insignificant compared to the area
	cout << data << endl;

	// It can even be useful if one feature has a greater role in clustering than the others. 
	// But if that’s not the case, scale them.
	double minv, maxv; 
	for (int i = 0; i < data.cols; ++i) {
		//data.col(i) contains the ith feature value for all buttons
		//this method will find the minimum and the maximum value
		minMaxLoc(data.col(i), &minv, &maxv);

		if (fabs(maxv - minv) > 0.000001)
			//rescaling: [minv, maxv] --> [0, 1]
			data.col(i) = (data.col(i) - minv) / (maxv - minv);
		else
			data.col(i).setTo(1.0);

	}

	cout << data << endl;

	// klaster the buttons based on the features
	// 1000 - maximum iteration
	// 0.01 - required minimum change
	// 3 - number of the attempts
	// KMEANS_RANDOM_CENTERS - the initial centers will be computed randomly
	Mat labels;
	kmeans(data, 5, labels,
		TermCriteria(TermCriteria::MAX_ITER, 1000, 0.01), 3, KMEANS_RANDOM_CENTERS);


	// let's see the result
	for (int i = 0; i < numObjs; ++i) {
		int lbl = labels.at<int>(i);  // the cluster of the ith top level object
		int ctrIdx = buttons[i]; //the index of the ith top level object of the "contours" vector
		Rect r = boundingRect(contours[ctrIdx]);
		// save the button: the first number in the filename is the cluster
		Mat simg = img(r);
		imwrite(to_string(lbl) + "_" + to_string(i) + ".png", simg);
	}

	//Note: The result may be slightly different for different runs, as kmeans randomly selects the centers.


	imshow("img", img);
	imshow("mask", mask);
	imwrite("b4_external_contours.png", rgb);
	imwrite("b5_boundingboxes_and_poligons.png", rgb2);
	waitKey(0);



	return 0;
}