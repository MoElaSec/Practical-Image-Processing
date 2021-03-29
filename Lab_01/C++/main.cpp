#include <iostream>
#include <opencv2/core.hpp>    // osztalyok, strukturak, alapveto funkciok
#include <opencv2/highgui.hpp> // felulet
#include <opencv2/imgproc.hpp> // kepfeld. konyvtar, gyakran kell majd

using namespace std;
using namespace cv;

int main() {
	
	Mat img = imread("fizu.jpg");
	imshow("jajj", img);
	waitKey(0);

	return 0;
}