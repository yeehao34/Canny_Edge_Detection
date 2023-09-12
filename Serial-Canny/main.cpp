#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cannyEdge.h"
#include <chrono> // time/clocks

using namespace std;

int low_threshold = 30;
int high_threshold = 90;

const char* CW_IMG_ORIGINAL = "Original";
const char* CW_IMG_GRAY = "Grayscale";
const char* CW_IMG_EDGE = "Canny Edge Detection";

void doTransform(std::string);

int main(int argc, char** argv) {
		cv::namedWindow(CW_IMG_ORIGINAL, cv::WINDOW_NORMAL);
		cv::namedWindow(CW_IMG_GRAY, cv::WINDOW_NORMAL);
		cv::namedWindow(CW_IMG_EDGE, cv::WINDOW_NORMAL);
		cv::resizeWindow(CW_IMG_ORIGINAL, 1280, 720);
		cv::resizeWindow(CW_IMG_GRAY, 1280, 720);
		cv::resizeWindow(CW_IMG_EDGE, 1280, 720);
		cv::moveWindow(CW_IMG_ORIGINAL, 10, 10);
		cv::moveWindow(CW_IMG_GRAY, 680, 10);
		cv::moveWindow(CW_IMG_EDGE, 1350, 10);

		int image_choice;
		std::cout << "Select image to do canny edge detection : " << std::endl;
		std::cout << "1. 640x480.jpg" << std::endl;
		std::cout << "2. 1280x720.jpg" << std::endl;
		std::cout << "3. 1920x1080.jpg" << std::endl;
		std::cout << "4. 2560x1440.jpg" << std::endl;
		std::cout << "5. 3840x2160.jpg" << std::endl;
		std::cout << "6. 7680x4320.jpg" << std::endl;
		std::cin >> image_choice;
		switch (image_choice)
		{
		case 1:
			doTransform("../img/640x480.jpg");
			break;
		case 2:
			doTransform("../img/1280x720.jpg");
			break;
		case 3:
			doTransform("../img/1920x1080.jpg");
			break;
		case 4:
			doTransform("../img/2560x1440.jpg");
			break;
		case 5:
			doTransform("../img/3840x2160.jpg");
			break;
		case 6:
			doTransform("../img/7680x4320.jpg");
			break;
		default:
			break;
		}

	cv::destroyAllWindows();
	return 0;
}

void doTransform(std::string file_path) {
	//cv::Mat img_edge;
	cv::Mat img_gray;

	// image read and convert the image into 3 channel BGR color img
	cv::Mat img_ori = cv::imread(file_path, 1);

	try {
		// 1st param: Original image
		// 2nd param: Destination image
		// cv::COLOR_BGR2GRAY --> color conversion code, convert from BGR 
		cv::cvtColor(img_ori, img_gray, cv::COLOR_BGR2GRAY);
	}
	catch (cv::Exception& e) {
		cerr << e.msg << endl;
	}
	int w = img_gray.cols;
	int h = img_ori.rows;

	// 8-bit unsigned integer image with 1 channel <-- grayscale 
	// Scalar:all(0) --> initializes all pixels to 0, creating a black image
	cv::Mat img_edge(h, w, CV_8UC1, cv::Scalar::all(0));

	//start timer
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	// (destination, source, low threshold, high threshold, width of the image, and height of the image)
	apply_canny(img_edge.data, img_gray.data, low_threshold, high_threshold, w, h);

	//stop timer
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
	// 640 x 480: Time taken by Serial implementation: 0.026303 seconds
	// 7680 x 4320: Time taken by Serial implementation: 2.686465 seconds
	printf("Time taken by Serial implementation: %.6f seconds\n", time_span);

	// Visualize all
	cv::imshow(CW_IMG_ORIGINAL, img_ori);
	cv::imshow(CW_IMG_GRAY, img_gray);
	cv::imshow(CW_IMG_EDGE, img_edge);

	// wait for user input within 6 minutes 
	char c = cv::waitKey(360000);

	if (c == 's') {
		cv::imwrite("canny.png", img_edge);
		std::cout << "write canny.png done..." << std::endl;
	}

	if (c == 27) return;

}
