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
    std::cout << "7. Run all" << std::endl;
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
    case 7:
        std::cout << "Running 640x480.jpg     :";
        doTransform("../img/640x480.jpg");
        std::cout << "Running 1280x720.jpg     :";
        doTransform("../img/1280x720.jpg");
        std::cout << "Running 1920x1080.jpg     :";
        doTransform("../img/1920x1080.jpg");
        std::cout << "Running 2560x1440.jpg     :";
        doTransform("../img/2560x1440.jpg");
        std::cout << "Running 3840x2160.jpg     :";
        doTransform("../img/3840x2160.jpg");
        std::cout << "Running 7680x4320.jpg     :";
        doTransform("../img/7680x4320.jpg");
        break;
    default:
        break;
    }

    cv::destroyAllWindows();
    return 0;
}

void doTransform(std::string file_path) {
    cv::Mat img_edge;
    cv::Mat img_gray;

    cv::Mat img_ori = cv::imread(file_path, 1);

    try {
        cv::cvtColor(img_ori, img_gray, cv::COLOR_BGR2GRAY);
    }
    catch (cv::Exception& e) {
        cerr << e.msg << endl;
    }
    int w = img_gray.cols;
    int h = img_ori.rows;


    while (1) {
        cv::Mat img_edge(h, w, CV_8UC1, cv::Scalar::all(0));

        //start timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        apply_canny(img_edge.data, img_gray.data, low_threshold, high_threshold, w, h);

        //stop timer
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Time taken by Serial implementation: %.6f\n", duration / 1000000.0f);

        // Visualize all
        cv::imshow(CW_IMG_ORIGINAL, img_ori);
        cv::imshow(CW_IMG_GRAY, img_gray);
        cv::imshow(CW_IMG_EDGE, img_edge);

        char c = cv::waitKey(360000);

        if (c == 'h') {
            if (high_threshold > 10)
                high_threshold -= 5;
            else
                high_threshold -= 1;
        }
        if (c == 'H') {
            if (high_threshold >= 10)
                high_threshold += 5;
            else
                high_threshold += 1;
        }
        if (c == 'l') {
            if (low_threshold > 10)
                low_threshold -= 5;
            else
                low_threshold -= 1;
        }
        if (c == 'L') {
            if (low_threshold >= 10)
                low_threshold += 5;
            else
                low_threshold += 1;
        }
        if (c == 's') {
            cv::imwrite("canny.png", img_edge);
            std::cout << "write canny.png done..." << std::endl;
        }

        std::cout << low_threshold << ", " << high_threshold << std::endl;

        if (c == 27) break;
    }
}
