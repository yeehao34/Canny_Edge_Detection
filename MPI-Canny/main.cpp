#include "stdio.h"
#include <iostream>
#include <iomanip>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cannyEdge.h"
#include <string>
#include "mpi.h"

int low_threshold = 20;
int high_threshold = 60;

const char* CW_IMG_ORIGINAL = "Original";
const char* CW_IMG_GRAY = "Grayscale";
const char* CW_IMG_EDGE = "Canny Edge Detection";

int main(int argc, char** argv) {
	int rank = 0, size = 0;
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	cv::Mat img_ori;
	cv::Mat img_gray;

	// set the file path
	std::string filepath = "";
	int w, h;

	// Receive image choice
	// Declare Ori, Grayscale, Edge Detection Window
	// Set filepath and read image, convert ori image to grayscale
	if (rank == 0) {
		int image_choice;
		std::cout << "       Image Dimensions      " << std::endl;
		std::cout << "#~~~~~~~~~~~~~~~~~~~~~~~~~~~~#" << std::endl;
		std::cout << "| " << std::left << std::setw(3) << "1." << std::setw(23) << "640x480.jpg" << " |" << std::endl;
		std::cout << "| " << std::left << std::setw(3) << "2." << std::setw(23) << "1280x720.jpg" << " |" << std::endl;
		std::cout << "| " << std::left << std::setw(3) << "3." << std::setw(23) << "1920x1080.jpg" << " |" << std::endl;
		std::cout << "| " << std::left << std::setw(3) << "4." << std::setw(23) << "2560x1440.jpg" << " |" << std::endl;
		std::cout << "| " << std::left << std::setw(3) << "5." << std::setw(23) << "3840x2160.jpg" << " |" << std::endl;
		std::cout << "#~~~~~~~~~~~~~~~~~~~~~~~~~~~~#" << std::endl;
		std::cout << "Select image to do canny edge detection : " << std::endl;
		fflush(stdout);
		// receive image choice 
		scanf_s("%d", &image_choice);
		// declare window, resize and move it
		cv::namedWindow(CW_IMG_ORIGINAL, cv::WINDOW_NORMAL);
		cv::namedWindow(CW_IMG_GRAY, cv::WINDOW_NORMAL);
		cv::namedWindow(CW_IMG_EDGE, cv::WINDOW_NORMAL);
		cv::resizeWindow(CW_IMG_ORIGINAL, 1280, 720);
		cv::resizeWindow(CW_IMG_GRAY, 1280, 720);
		cv::resizeWindow(CW_IMG_EDGE, 1280, 720);
		cv::moveWindow(CW_IMG_ORIGINAL, 10, 10);
		cv::moveWindow(CW_IMG_GRAY, 680, 10);
		cv::moveWindow(CW_IMG_EDGE, 1350, 10);


		switch (image_choice)
		{
		case 1:
			filepath = "640x480.jpg";
			break;
		case 2:
			filepath = "1280x720.jpg";
			break;
		case 3:
			filepath = "1920x1080.jpg";
			break;
		case 4:
			filepath = "2560x1440.jpg";
			break;
		case 5:
			filepath = "3840x2160.jpg";
			break;
		case 6:
			filepath = "7680x4320.jpg";
			break;
		default:
			break;
		}

		// read image and convert the image into 3 channel BGR color img
		img_ori = cv::imread("../img/" + filepath, 1);

		try {
			// 1st param: Original image
			// 2nd param: Destination image
			// cv::COLOR_BGR2GRAY --> color conversion code, convert from BGR 
			cv::cvtColor(img_ori, img_gray, cv::COLOR_BGR2GRAY);
		}
		catch (cv::Exception& e) {
			std::cerr << e.msg << std::endl;
		}
		w = img_gray.cols;
		h = img_ori.rows;
	}

	// start process here
	MPI_Bcast(&w, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&h, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	double begin = MPI_Wtime();

	int* scounts = (int*)malloc(size * sizeof(int));
	int* displs = (int*)malloc(size * sizeof(int));
	assert(scouts != NULL);
	assert(displs != NULL);

	// Assign each process should handle how many row
	int rows_per_process = h / size;
	int remainder = h % size;

	for (int i = 0; i < size; i++) {
		if (i == size - 1) {
			// Last process handle the extra rows
			scounts[i] = rows_per_process + remainder;
		}
		else {
			// Others will get the same number of rows
			scounts[i] = rows_per_process;
		}
		scounts[i] *= w;

		displs[i] = (i > 0) ? displs[i - 1] + scounts[i - 1] : 0;
	}


	// Data being distributed to each process
	cv::Mat local_img(scounts[rank], w, CV_8UC1);

	// Distribute image chunks between processors
	MPI_Scatterv(img_gray.data, scounts, displs, MPI_UNSIGNED_CHAR, local_img.data, scounts[rank], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);


	cv::Mat partialImg(rows_per_process, w, CV_8UC1);

	apply_canny(partialImg.data, local_img.data, low_threshold, high_threshold, w, rows_per_process);

	cv::Mat processed_img;

	if (rank == 0) {
		processed_img = cv::Mat(h, w, CV_8UC1);
	}

	MPI_Gatherv(partialImg.data, scounts[rank], MPI_UNSIGNED_CHAR, processed_img.data, scounts, displs, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	double end = MPI_Wtime();
	double time_elapsed = end - begin;


	if (rank == 0) {
		// Display the time taken for execute
		// 4 process:
		// 640 x 480 - Time taken by MPI Implementation: 0.006716 seconds
		// 7680 x 4320 - Time taken by MPI Implementation: 0.571860 seconds
		printf("Time taken by MPI Implementation: %.6f seconds\n", time_elapsed);
		// Visualize all
		cv::imshow(CW_IMG_ORIGINAL, img_ori);
		cv::imshow(CW_IMG_GRAY, img_gray);
		cv::imshow(CW_IMG_EDGE, processed_img);

		std::string true_path = "../img/true/" + filepath;
		std::string save_path = "saved/" + filepath;

		cv::imwrite(save_path, processed_img);
		cv::Mat test_img_true = cv::imread(true_path, 1);
		cv::Mat test_img_edge = cv::imread(save_path, 1);

		char c = cv::waitKey(360000);

		if (c == 27) {
			cv::destroyAllWindows();
		}
	}
	MPI_Finalize();
	free(scounts);
	free(displs);

	return 0;
}