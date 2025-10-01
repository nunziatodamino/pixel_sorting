#pragma once
#include <CLI11.hpp>
#include <opencv2/opencv.hpp>

struct Config
{
  std::string input_file;
  std::string output_file;
  bool use_random_sort = false;
  bool use_vertical_sort = false;
  bool use_blue_channel = false;
  int threshold = 0;
  float relEntropy = 0.0f;
  bool use_horizontal_sort = false;
  bool write = false;
  
  static constexpr int maxAbsBrightness{255+255+255};
};

void cliSetup (CLI::App& app, Config& config);
cv::Mat loadImage(const Config& config); 
void applyImageProcessing(cv::Mat& img, Config& config);
void displayImage(cv::Mat& img);

