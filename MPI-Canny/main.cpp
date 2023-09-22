//#include "stdio.h"
//#include <iostream>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "cannyEdge.h"
//#include <string>
//#include "mpi.h"
//#define KERNEL_SIZE 3
//
//int low_threshold = 30;
//int high_threshold = 90;
//
//const char* CW_IMG_ORIGINAL = "Original";
//const char* CW_IMG_GRAY = "Grayscale";
//const char* CW_IMG_EDGE = "Canny Edge Detection";
//
//bool image_equal(const cv::Mat& a, const cv::Mat& b);
//
//int main(int argc, char** argv) {
//	int world_rank = 0, world_size = 0;
//	MPI_Init(&argc, &argv);
//
//	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//
//	cv::Mat img_ori;
//	cv::Mat img_gray;
//	cv::Mat img_edge;
//	int rowSize;
//	uchar* data;		// image data
//	int* rowSizes = (int*)malloc(sizeof(int*) * world_size);		// vector containing image row split sizes for each process
//	int params[2];		// parameter vector for processes (w, rowSize)
//	uchar* section;		// partial image section for each process
//	uchar* partialEdge;	// edged image section from each process
//	int num;			// number of rows to assign each process
//	int temp;			// remaining rows incase of uneven division
//	int offset;
//
//	// set the file path
//	std::string filepath = "";
//	int w, h;
//
//	// Receive image choice
//	// Declare Ori, Grayscale, Edge Detection Window
//	// Set filepath and read image, convert ori image to grayscale
//	if (world_rank == 0) {
//		int image_choice;
//		std::cout << "Select image to do canny edge detection : " << std::endl;
//		std::cout << "1. 640x480.jpg" << std::endl;
//		std::cout << "2. 1280x720.jpg" << std::endl;
//		std::cout << "3. 1920x1080.jpg" << std::endl;
//		std::cout << "4. 2560x1440.jpg" << std::endl;
//		std::cout << "5. 3840x2160.jpg" << std::endl;
//		std::cout << "6. 7680x4320.jpg" << std::endl;
//		fflush(stdout);
//		// receive image choice 
//		scanf_s("%d", &image_choice);
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
//
//		switch (image_choice)
//		{
//		case 1:
//			filepath = "640x480.jpg";
//			break;
//		case 2:
//			filepath = "1280x720.jpg";
//			break;
//		case 3:
//			filepath = "1920x1080.jpg";
//			break;
//		case 4:
//			filepath = "2560x1440.jpg";
//			break;
//		case 5:
//			filepath = "3840x2160.jpg";
//			break;
//		case 6:
//			filepath = "7680x4320.jpg";
//			break;
//		default:
//			break;
//		}
//
//		// read image and convert the image into 3 channel BGR color img
//		img_ori = cv::imread("../img/" + filepath, 1);
//
//		try {
//			// 1st param: Original image
//			// 2nd param: Destination image
//			// cv::COLOR_BGR2GRAY --> color conversion code, convert from BGR 
//			cv::cvtColor(img_ori, img_gray, cv::COLOR_BGR2GRAY);
//		}
//		catch (cv::Exception& e) {
//			std::cerr << e.msg << std::endl;
//		}
//		w = img_gray.cols;
//		h = img_ori.rows;
//		data = img_gray.data;
//
//		// int array to create partition size for each process
//		//rowSizes = (int*)malloc(sizeof(int*) * world_size);
//		num = h / world_size;
//		temp = h % world_size;	// see whether there is any extra rows
//		for (int x = world_size = 1; x >= 0; x--) {
//			rowSizes[x] = num;
//		}
//		// let the last process to handle the extra rows
//		if (temp > 0) {
//			rowSizes[world_size - 1] += temp;
//		}
//
//		rowSize = rowSizes[0];
//		// send w and rows of each process to processes > 0
//		params[0] = w;
//		for (int i = 1; i < world_size; i++) {
//			params[1] = rowSizes[i];
//			MPI_Send(params, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
//		}
//
//		// send sections of image to processes > 0
//		offset = rowSize;
//		for (int i = 1; i < world_size; i++) {
//			temp = w * rowSizes[i];	// total number of elements in a section
//			section = (uchar*)malloc(sizeof(uchar*) * temp);
//			memcpy(section, data + w * offset, temp);
//			MPI_Send(section, temp, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
//			free(section);
//			offset += rowSizes[i];
//		}
//
//		// copy section for process 0
//		temp = w * rowSize;
//		section = (uchar*)malloc(sizeof(uchar*) * temp);
//		memcpy(section, data, temp);
//
//	}
//	else {	// for processes > 0
//		// receive params from process 0
//		MPI_Recv(params, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//		w = params[0];
//		rowSize = params[1];
//
//		// receive image section from process 0
//		temp = w * rowSize;
//		section = (uchar*)malloc(sizeof(uchar*) * temp);
//		MPI_Recv(section, w * rowSize, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//
//	}
//
//	// start edge detection 
//
//	MPI_Barrier(MPI_COMM_WORLD);
//	double begin = MPI_Wtime();
//
//	// create local image for each process
//	cv::Mat local_img(rowSize, w, CV_8UC1);
//	apply_canny(local_img.data, section, low_threshold, high_threshold, w, rowSize);
//	free(section);
//
//	// send local_img.data back to process 0
//	if (world_rank != 0) {
//		partialEdge = local_img.data;
//		temp = w * rowSize;
//		MPI_Send(partialEdge, temp, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
//	}
//	else {	// for process 0
//		img_edge = cv::Mat(h, w, CV_8UC1);
//		temp = w * rowSize;
//		memcpy(img_edge.data + w, local_img.data, temp);
//
//		// receive processed image sections and concatenate with img_edge
//		offset = rowSize;
//		for (int i = 1; i < world_size; i++) {
//			temp = w * rowSizes[i];
//			partialEdge = (uchar*)malloc(sizeof(uchar*) * temp);
//			MPI_Recv(partialEdge, temp, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//			memcpy(img_edge.data + w * offset, partialEdge, temp);
//			free(partialEdge);
//			offset += rowSizes[i];
//		}
//	}
//
//
//	//int rows_per_rank = h / world_size;
//
//	//// Data being distributed to each process
//	//cv::Mat local_img(rows_per_rank, w, CV_8UC1);
//
//	//// Distribute image chunks between processors
//	//MPI_Scatter(img_gray.data, w * rows_per_rank, MPI_UNSIGNED_CHAR, local_img.data, w * rows_per_rank, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
//
//	//cv::Mat partialImg(rows_per_rank, w, CV_8UC1);
//
//	//apply_canny(partialImg.data, local_img.data, low_threshold, high_threshold, w, rows_per_rank);
//
//	//cv::Mat processed_img;
//
//	//if (world_rank == 0) {
//	//	processed_img = cv::Mat(h, w, CV_8UC1);
//	//}
//
//	//MPI_Gather(partialImg.data, w * rows_per_rank, MPI_UNSIGNED_CHAR, processed_img.data, w * rows_per_rank, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
//	MPI_Barrier(MPI_COMM_WORLD);
//	double end = MPI_Wtime();
//	double time_elapsed = end - begin;
//
//
//	if (world_rank == 0) {
//		// Display the time taken for execute
//		// 4 process:
//		// 640 x 480 - Time taken by MPI Implementation: 0.006716 seconds
//		// 7680 x 4320 - Time taken by MPI Implementation: 0.571860 seconds
//		printf("Time taken by MPI Implementation: %.6f seconds\n", time_elapsed);
//		// Visualize all
//		cv::imshow(CW_IMG_ORIGINAL, img_ori);
//		cv::imshow(CW_IMG_GRAY, img_gray);
//		cv::imshow(CW_IMG_EDGE, img_edge);
//
//		std::string true_path = "../img/true/" + filepath;
//		std::string save_path = "saved/" + filepath;
//
//		cv::imwrite(save_path, img_edge);
//		cv::Mat test_img_true = cv::imread(true_path, 1);
//		cv::Mat test_img_edge = cv::imread(save_path, 1);
//
//		// Dk why not working - value not equal to 0 --> because there is a gap between the processes
//		if (image_equal(test_img_edge, test_img_true))
//		{
//			std::cout << "correct edge result" << std::endl;
//		}
//
//		char c = cv::waitKey(360000);
//
//		if (c == 27) {
//			cv::destroyAllWindows();
//		}
//	}
//	MPI_Finalize();
//
//	return 0;
//}
//
//bool image_equal(const cv::Mat& a, const cv::Mat& b)
//{
//	if ((a.rows != b.rows) || (a.cols != b.cols)) {
//		return false;
//	}
//	cv::Scalar s = sum(a - b);
//	std::cout << s[0] << "+" << s[1] << "+" << s[2] << std::endl;
//	return (s[0] == 0) && (s[1] == 0) && (s[2] == 0);
//}
