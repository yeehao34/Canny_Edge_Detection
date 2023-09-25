#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cannyEdge.h"
#include <string>
#include <iomanip>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

int low_threshold = 20;
int high_threshold = 60;

const char *CW_IMG_ORIGINAL = "Original";
const char *CW_IMG_GRAY = "Grayscale";
const char *CW_IMG_EDGE = "Canny Edge Detection";

void doTransform(std::string, int);

__global__ void cuda_hello()
{
	printf("Hello World from GPU!\n");
}
int main(int argc, char **argv)
{
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

	// std::string img_path = "img/";
	int image_choice;
	std::cout << "       Image Dimensions      " << std::endl;
	std::cout << "#~~~~~~~~~~~~~~~~~~~~~~~~~~~~#" << std::endl;
	std::cout << "| " << std::left << std::setw(3) << "1." << std::setw(23) << "640x480.jpg"
			  << " |" << std::endl;
	std::cout << "| " << std::left << std::setw(3) << "2." << std::setw(23) << "1280x720.jpg"
			  << " |" << std::endl;
	std::cout << "| " << std::left << std::setw(3) << "3." << std::setw(23) << "1920x1080.jpg"
			  << " |" << std::endl;
	std::cout << "| " << std::left << std::setw(3) << "4." << std::setw(23) << "2560x1440.jpg"
			  << " |" << std::endl;
	std::cout << "| " << std::left << std::setw(3) << "5." << std::setw(23) << "3840x2160.jpg"
			  << " |" << std::endl;
	std::cout << "#~~~~~~~~~~~~~~~~~~~~~~~~~~~~#" << std::endl;
	std::cout << "Select image to do canny edge detection : " << std::endl;
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
	default:
		break;
	}
	doTransform(filepath + ".jpg", thd_per_blk);

	cv::destroyAllWindows();
	return 0;
}

void doTransform(std::string file_path, int thd_per_blk)
{
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

	// Visualize all
	cv::imshow(CW_IMG_ORIGINAL, img_ori);
	cv::imshow(CW_IMG_GRAY, img_gray);
	cv::imshow(CW_IMG_EDGE, img_edge);

	char c = cv::waitKey(360000);
}
