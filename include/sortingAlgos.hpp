#pragma once 
#include <opencv2/opencv.hpp>

void sortByColumnCPU(cv::Mat& img);
void randomSortCPU(cv::Mat& img, float relEntropy);
void imagePrint(cv::Mat& img);