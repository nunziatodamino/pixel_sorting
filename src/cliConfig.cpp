#include <CLI11.hpp>
#include "sortingAlgos.hpp"

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
  
  CLI::TransformPairs<Config::Mode> mode_map
  {
    {"horizontal", Config::Mode::Horizontal},
    {"vertical",   Config::Mode::Vertical},
    {"random",     Config::Mode::RandomSort}
  };
  app.add_option("-m, --method", config.mode, "Sorting method")
        ->required()
       ->transform(CLI::Transformer(mode_map, CLI::ignore_case));
  
  CLI::TransformPairs<Config::ColorSpace> color_map
  {
    {"HSV",  Config::ColorSpace::HSV},
    {"LAB",  Config::ColorSpace::LAB},
    {"YCrCb",Config::ColorSpace::YCrCB}
  };
  app.add_option("-c, --color", config.colorSpace, "Select color space")
       ->transform(CLI::Transformer(color_map, CLI::ignore_case));

  app.add_option("-t,--treshold", config.threshold, "Use threshold on brightness with sort")
        ->expected(0, config.maxAbsBrightness)
        ->check(CLI::Range(0, config.maxAbsBrightness));
  app.add_option("-e,--entropy", config.relEntropy, "set relative entropy for the random sort")
        ->expected(0, 1);
  app.add_flag("-w,--write", config.write, "Write result to output file");
}

void transformImage(cv::Mat& img, Config& config)
{
  switch (config.colorSpace)
  {
  case Config::ColorSpace::HSV:
    cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
    break;
  case Config::ColorSpace::LAB:
    cv::cvtColor(img, img, cv::COLOR_BGR2Lab);
    break;
  case Config::ColorSpace::YCrCB:
    cv::cvtColor(img, img, cv::COLOR_BGR2YCrCb);
    break;  
  default: // does nothing as the image is already BGR
    break;
  }
}

void applyImageProcessing(cv::Mat& img, Config& config)
{
  switch (config.mode)
  {
  case Config::Mode::Horizontal:
    if (config.threshold >0) {sortByRowThresholdCPU(img, config.threshold);}
    else {sortByRowThresholdCPU(img, 0);}
    break;
  case Config::Mode::Vertical:
    if (config.threshold >0) {sortByColumnThresholdCPU(img, config.threshold);}
    else {sortByColumnThresholdCPU(img, 0);}
    break;
  case Config::Mode::RandomSort:
       if (config.relEntropy >= 0){randomSortCPU(img, config.relEntropy);}
       break;
  default: 
       throw std::runtime_error("Sorting method not specified.\n");
       break;
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

void displayImage(cv::Mat& img)
{
  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image", img);
  cv::waitKey(0);
}
