//#define _USE_MATH_DEFINES 
//#include <cmath>
//#include <iostream>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "cannyEdge.h"
//#include <mpi.h>
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
//int main(int argc, char** argv) {
//	MPI_Init(&argc, &argv);
//	int rank, size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//	cv::Mat img_gray;
//	cv::Mat img_ori;
//
//	// Receive image choice
//	// Declare Ori, Grayscale, Edge Detection Window
//	// Set filepath and read image, convert ori image to grayscale
//	if (rank == 0) {
//		int image_choice;
//		std::cout << "Select image to do canny edge detection : " << std::endl;
//		std::cout << "1. 640x480.jpg" << std::endl;
//		std::cout << "2. 1280x720.jpg" << std::endl;
//		std::cout << "3. 1920x1080.jpg" << std::endl;
//		std::cout << "4. 2560x1440.jpg" << std::endl;
//		std::cout << "5. 3840x2160.jpg" << std::endl;
//		std::cout << "6. 7680x4320.jpg" << std::endl;
//		// receive image choice 
//		std::cin >> image_choice;
//		// declare window, resize and move it
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
//		// set the file path
//		string filepath = "";
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
//
//		filepath = "../img/" + filepath + ".jpg";
//
//		// read image and convert the image into 3 channel BGR color img
//		img_ori = cv::imread(filepath, 1);
//
//		try {
//			// 1st param: Original image
//			// 2nd param: Destination image
//			// cv::COLOR_BGR2GRAY --> color conversion code, convert from BGR 
//			cv::cvtColor(img_ori, img_gray, cv::COLOR_BGR2GRAY);
//		}
//		catch (cv::Exception& e) {
//			cerr << e.msg << endl;
//		}
//	}
//
//	//MPI_Bcast((void*)filepath.c_str(), filepath.length(), MPI_CHAR, 0, MPI_COMM_WORLD);
//
//	// Broadcast the image size
//	int w, h;
//	if (rank == 0) {
//		w = img_gray.cols;
//		h = img_ori.rows;
//	}
//
//	// MPI_Bcast(*buffer, count, MPI_Datatype, root, MPI_Comm) 
//	MPI_Bcast(&w, 1, MPI_INT, 0, MPI_COMM_WORLD);
//	MPI_Bcast(&h, 1, MPI_INT, 0, MPI_COMM_WORLD);
//
//	img_ori.elemSize();
//
//	// Determine the image chunk size for each process
//	int rows_per_process = h / size;
//	int remainder = h % size;
//
//	// Calculate the start and end rows for each process
//	int start_row = rank * rows_per_process;
//	int end_row = start_row + rows_per_process;
//	if (rank == size - 1) {
//		end_row += remainder; // Last process handles the remainder
//	}
//
//	// Create a local image chunk for each process
//	cv::Mat img_chunk_edge(h, w, CV_8UC1, cv::Scalar::all(0));
//
//	// Scatter the image data to each process
//	if (rank == 0) {
//		for (int i = 1; i < size; ++i) {
//			int chunk_start = i * rows_per_process;
//			int chunk_end = chunk_start + rows_per_process;
//			if (i == size - 1) {
//				chunk_end += remainder;
//			}
//			MPI_Send(img_gray.data + chunk_start * w, (chunk_end - chunk_start) * w, MPI_BYTE, i, 0, MPI_COMM_WORLD);
//		}
//	}
//	else {
//		MPI_Recv(img_chunk_edge.data + start_row * w, (end_row - start_row) * w, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//	} 
//
//	// (destination, source, low threshold, high threshold, width of the image, and height of the image)
//	apply_canny(img_chunk_edge.data, img_gray.data, low_threshold, high_threshold, w, end_row - start_row);
//
//	// Gather the processed image chunks back to rank 0
//	if (rank == 0) {
//		for (int i = 1; i < size; ++i) {
//			int chunk_start = i * rows_per_process;
//			int chunk_end = chunk_start + rows_per_process;
//			if (i == size - 1) {
//				chunk_end += remainder;
//			}
//			MPI_Recv(img_chunk_edge.data + chunk_start * w, (chunk_end - chunk_start) * w, MPI_BYTE, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//		}
//	}
//	else {
//		MPI_Send(img_chunk_edge.data + start_row * w, (end_row - start_row) * w, MPI_BYTE, 0, 1, MPI_COMM_WORLD);
//	}
//
//	//printf("Time taken by MPI Implementation: %.6f\n", );
//
//	// Visualize all
//	if (rank == 0) {
//		cv::imshow(CW_IMG_ORIGINAL, img_ori);
//		cv::imshow(CW_IMG_GRAY, img_gray);
//		cv::imshow(CW_IMG_EDGE, img_chunk_edge);
//
//		// wait for user input within 6 minutes 
//		char c = cv::waitKey(360000);
//
//		if (c == 's') {
//			cv::imwrite("canny.png", img_chunk_edge);
//			std::cout << "write canny.png done..." << std::endl;
//		}
//	}
//
//	cv::destroyAllWindows();
//	MPI_Barrier(MPI_COMM_WORLD);
//	MPI_Finalize();
//	return 0;
//}
