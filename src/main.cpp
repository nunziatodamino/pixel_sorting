#include <CLI11.hpp>
#include "sortingAlgos.hpp"


int main(int argc, char** argv )
{
  CLI::App app{"PixelSort"}; 
  
  std::string input_file;
  std::string output_file;
  bool use_random_sort = false;
  bool write = false;
  
  app.add_option("-i,--input", input_file, "Input image file")
        ->required()
        ->check(CLI::ExistingFile);
  app.add_option("-o,--output", output_file, "Output image file")
        ->required();
  app.add_flag("-r,--random", use_random_sort, "Use random sort");
  app.add_flag("-w,--write", write, "Write result to output file");

  CLI11_PARSE(app, argc, argv);
  
  cv::Mat img = cv::imread( input_file, cv::IMREAD_COLOR );
  if (img.empty()) 
  {
    std::cerr << "Error: Failed to load image from " << input_file << std::endl; 
    return 1;
  }
  if (use_random_sort){randomSortCPU(img, 0.85f);}
  if (write)
  {
    if (output_file.empty())
    {
      std::cerr << "Output file not specified.\n";
      return 1;
    }
    cv::imwrite(output_file, img);
  }
  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image", img);
  cv::waitKey(0);
}