#include "cliConfig.hpp"

int main(int argc, char** argv )
{
  CLI::App app{"PixelSort"}; 
  Config configData{};
  cliSetup(app, configData);
  CLI11_PARSE(app, argc, argv);
  /*
  cv::Mat img = cv::imread( input_file, cv::IMREAD_COLOR );
  if (img.empty()) 
  {
    std::cerr << "Error: Failed to load image from " << input_file << std::endl; 
    return 1;
  }
  */
  cv::Mat img = loadImage(configData);
  applyImageProcessing(img, configData);

  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image", img);
  cv::waitKey(0);
}