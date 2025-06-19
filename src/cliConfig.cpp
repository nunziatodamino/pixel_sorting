#include <CLI11.hpp>
#include "sortingAlgos.hpp"

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

cv::Mat loadImage(const Config& config) 
{
    cv::Mat img = cv::imread(config.input_file, cv::IMREAD_COLOR);
    if (img.empty()) {
        throw std::runtime_error("Failed to load image from " + config.input_file);
    }
    return img;
}

void cliSetup (CLI::App& app, Config& config)
{
  app.add_option("-i,--input", config.input_file, "Input image file")
        ->required()
        ->check(CLI::ExistingFile);
  app.add_option("-o,--output", config.output_file, "Output image file")
        ->required();
  app.add_option("-t,--treshold", config.threshold, "Use threshold on brightness with sort")
        ->expected(0, config.maxAbsBrightness)
        ->check(CLI::Range(0, config.maxAbsBrightness));
  app.add_option("-e,--entropy", config.relEntropy, "set relative entropy for the random sort")
        ->expected(0, 1);
 
  app.add_flag("-r,--random", config.use_random_sort, "Use random sort");
  app.add_flag("-v,--vertical", config.use_vertical_sort, "Use vertical sort");
  app.add_flag("-x,--horizontal", config.use_horizontal_sort, "Use horizontal sort");
  app.add_flag("-b,--blue", config.use_blue_channel, "Output just the blue channel");
  app.add_flag("-w,--write", config.write, "Write result to output file");
}

void applyImageProcessing(cv::Mat& img, Config& config)
{
    if (config.use_blue_channel){blueChannel(img);}
  if (config.use_random_sort)
    {
      if (config.relEntropy >= 0){randomSortCPU(img, config.relEntropy);}
    }
  if (config.use_vertical_sort)
    {
      if (config.threshold > 0){sortByColumnThresholdCPU(img, config.threshold);}
      else{sortByColumnThresholdCPU(img, 0);}
    }
  if (config.use_horizontal_sort)
    {
      if (config.threshold >0) {sortByRowThresholdCPU(img, config.threshold);}
      else {sortByRowThresholdCPU(img, 0);}
    }
  if (config.write)
  {
    if (config.output_file.empty())
    {
      throw std::runtime_error("Output file not specified.\n");
    }
    cv::imwrite(config.output_file, img);
  }
}
