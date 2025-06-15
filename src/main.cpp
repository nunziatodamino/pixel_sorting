#include "sortingAlgos.hpp"

int main(int argc, char** argv )
{
  cv::Mat img;
  img = cv::imread( argv[1], cv::IMREAD_COLOR );
  if (imageInitError(img, argc, argv)!=0) return -1;
  randomSortCPU(img, 0.85f);
  imagePrint(img);  
}