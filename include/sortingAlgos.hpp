#pragma once 
#include <opencv2/opencv.hpp>

int imageInitError(cv::Mat& img, int argc, char** argv);
void sortByColumnCPU(cv::Mat& img);
void imagePrint(cv::Mat& img);