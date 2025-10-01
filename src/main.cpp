#include "cliConfig.hpp"

int main(int argc, char** argv )
{
  // CLI logic 
  CLI::App app{"PixelSort"}; 
  Config configData{};
  cliSetup(app, configData);
  CLI11_PARSE(app, argc, argv);
  
  // Image processing
  cv::Mat img = loadImage(configData);
  applyImageProcessing(img, configData);
  displayImage(img);
}
