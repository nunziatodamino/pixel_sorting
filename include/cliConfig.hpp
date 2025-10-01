#pragma once
#include <CLI11.hpp>
#include <opencv2/opencv.hpp>

struct Config
{
  enum class Mode
  {
    NoMethod,
    Horizontal,
    Vertical,
    RandomSort,
  };

  enum class ColorSpace
  {
    NoTransformation, // BGR space
    HSV,
    LAB,
    YCrCB,
  };

  std::string input_file;
  std::string output_file;
  Mode mode;
  ColorSpace colorSpace;
  int threshold = 0;
  float relEntropy = 0.0f;
  bool write = false;
  
  static constexpr int maxAbsBrightness{255+255+255};
};

void cliSetup (CLI::App& app, Config& config);
cv::Mat loadImage(const Config& config); 
void applyImageProcessing(cv::Mat& img, Config& config);
void displayImage(cv::Mat& img);

