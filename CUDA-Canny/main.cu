﻿#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cannyEdge.h"
#include <string>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

int low_threshold = 30;
int high_threshold = 90;

const char* CW_IMG_ORIGINAL = "Original";
const char* CW_IMG_GRAY = "Grayscale";
const char* CW_IMG_EDGE = "Canny Edge Detection";

void doTransform(std::string, int);

__global__ void cuda_hello() {
	printf("Hello World from GPU!\n");
}
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

	int thd_per_blk;
	std::cout << "Please enter the number of threads per block :" << std::endl;
	std::cin >> thd_per_blk;

	//std::string img_path = "img/";
	int image_choice;
	std::cout << "Select image to do canny edge detection : " << std::endl;
	std::cout << "1. 640x480.jpg" << std::endl;
	std::cout << "2. 1280x720.jpg" << std::endl;
	std::cout << "3. 1920x1080.jpg" << std::endl;
	std::cout << "4. 2560x1440.jpg" << std::endl;
	std::cout << "5. 3840x2160.jpg" << std::endl;
	std::cout << "6. 7680x4320.jpg" << std::endl;
	std::cout << "7. Run all" << std::endl;
	std::cin >> image_choice;
	std::string filepath = "";
	switch (image_choice)
	{
	case 1:
		filepath = "640x480";
		break;
	case 2:
		filepath = "1280x720";
		break;
	case 3:
		filepath = "1920x1080";
		break;
	case 4:
		filepath = "2560x1440";
		break;
	case 5:
		filepath = "3840x2160";
		break;
	case 6:
		filepath = "7680x4320";
		break;
	default:
		break;
	}
	doTransform(filepath + ".jpg", thd_per_blk);

	cv::destroyAllWindows();
	return 0;
}

bool image_equal(const cv::Mat& a, const cv::Mat& b)
{
	if ((a.rows != b.rows) || (a.cols != b.cols))
		return false;
	cv::Scalar s = sum(a - b);
	return (s[0] == 0) && (s[1] == 0) && (s[2] == 0);
}

void doTransform(std::string file_path, int thd_per_blk) {
	cv::Mat img_gray;
	std::string true_path = "../img/true/" + file_path;
	std::string save_path = "saved/" + file_path;

	cv::Mat img_ori = cv::imread("../img/" + file_path, 1);
	cv::cvtColor(img_ori, img_gray, cv::COLOR_BGR2GRAY);

	int w = img_gray.cols;
	int h = img_ori.rows;

	cv::Mat img_edge(h, w, CV_8UC1, cv::Scalar::all(0));
	apply_canny(img_edge.data, img_gray.data, low_threshold, high_threshold, w, h, thd_per_blk);

	cv::imwrite(save_path, img_edge);
	cv::Mat test_img_true = cv::imread(true_path, 1);
	cv::Mat test_img_edge = cv::imread(save_path, 1);

	if (image_equal(test_img_edge, test_img_true))
	{
		std::cout << "correct edge result" << std::endl;
	}


	// Visualize all
	cv::imshow(CW_IMG_ORIGINAL, img_ori);
	cv::imshow(CW_IMG_GRAY, img_gray);
	cv::imshow(CW_IMG_EDGE, img_edge);

	char c = cv::waitKey(360000);

	if (c == 27) return;

}
