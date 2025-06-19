#pragma once 
#include <opencv2/opencv.hpp>

void blueChannel(cv::Mat& img);
void sortByColumnThresholdCPU(cv::Mat& img, float threshold);
void sortByRowThresholdCPU(cv::Mat& img, float threshold);
void randomSortCPU(cv::Mat& img, float relEntropy);
void imagePrint(cv::Mat& img);