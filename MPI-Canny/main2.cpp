//#define _USE_MATH_DEFINES 
//#include <cmath>
//#include <iostream>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "cannyEdge.h"
//#include <mpi.h>
//#include <string>
//
//using namespace std;
//
//int low_threshold = 30;
//int high_threshold = 90;
//
//const char* CW_IMG_ORIGINAL = "Original";
//const char* CW_IMG_GRAY = "Grayscale";
//const char* CW_IMG_EDGE = "Canny Edge Detection";
//
//void doTransform(std::string);
//
//int main(int argc, char** argv) {
//	MPI_Init(&argc, &argv);
//	int rank, size;
//	//Get process ID
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//
//	//Get total processes Number
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//	string filepath = "";
//	int image_choice = 0;
//	if (rank == 0) {
//		cv::namedWindow(CW_IMG_ORIGINAL, cv::WINDOW_NORMAL);
//		cv::namedWindow(CW_IMG_GRAY, cv::WINDOW_NORMAL);
//		cv::namedWindow(CW_IMG_EDGE, cv::WINDOW_NORMAL);
//		cv::resizeWindow(CW_IMG_ORIGINAL, 1280, 720);
//		cv::resizeWindow(CW_IMG_GRAY, 1280, 720);
//		cv::resizeWindow(CW_IMG_EDGE, 1280, 720);
//		cv::moveWindow(CW_IMG_ORIGINAL, 10, 10);
//		cv::moveWindow(CW_IMG_GRAY, 680, 10);
//		cv::moveWindow(CW_IMG_EDGE, 1350, 10);
//
//		std::cout << "Select image to do canny edge detection : " << std::endl;
//		std::cout << "1. 640x480.jpg" << std::endl;
//		std::cout << "2. 1280x720.jpg" << std::endl;
//		std::cout << "3. 1920x1080.jpg" << std::endl;
//		std::cout << "4. 2560x1440.jpg" << std::endl;
//		std::cout << "5. 3840x2160.jpg" << std::endl;
//		std::cout << "6. 7680x4320.jpg" << std::endl;
//		std::cin >> image_choice;
//		switch (image_choice)
//		{
//		case 1:
//			filepath = "640x480";
//			break;
//		case 2:
//			filepath = "1280x720";
//			break;
//		case 3:
//			filepath = "1920x1080";
//			break;
//		case 4:
//			filepath = "2560x1440";
//			break;
//		case 5:
//			filepath = "3840x2160";
//			break;
//		case 6:
//			filepath = "7680x4320";
//			break;
//		default:
//			break;
//		}
//		filepath = "../img/" + filepath + ".jpg";
//	}
//
//	MPI_Bcast((void*)filepath.c_str(), filepath.length(), MPI_CHAR, 0, MPI_COMM_WORLD);
//
//	// start process here
//	cv::Mat img_gray;
//
//	cv::Mat img_ori = cv::imread(filepath, 1);
//
//	try {
//		cv::cvtColor(img_ori, img_gray, cv::COLOR_BGR2GRAY);
//	}
//	catch (cv::Exception& e) {
//		cerr << e.msg << endl;
//	}
//	int w = img_gray.cols;
//	int h = img_ori.rows;
//
//	cv::Mat img_edge(h, w, CV_8UC1, cv::Scalar::all(0));
//	MPI_Barrier(MPI_COMM_WORLD);
//	double begin = MPI_Wtime();
//
//	apply_canny(img_edge.data, img_gray.data, low_threshold, high_threshold, w, h);
//
//	MPI_Barrier(MPI_COMM_WORLD);
//	double end = MPI_Wtime();
//
//	// Display the time taken for execute
//	if (rank == 0) {
//		printf("Time taken by MPI implementation: %.6f\n", end - begin);
//	}
//
//	if (rank == 0) {
//		// Visualize all
//		cv::imshow(CW_IMG_ORIGINAL, img_ori);
//		cv::imshow(CW_IMG_GRAY, img_gray);
//		cv::imshow(CW_IMG_EDGE, img_edge);
//	}
//
//	char c = cv::waitKey(360000);
//	if (rank == 0) {
//		if (c == 's') {
//			cv::imwrite("canny.png", img_edge);
//			std::cout << "write canny.png done..." << std::endl;
//		}
//
//		if (c == 27) {
//			cv::destroyAllWindows();
//		}
//	}
//	MPI_Finalize();
//
//	return 0;
//}
