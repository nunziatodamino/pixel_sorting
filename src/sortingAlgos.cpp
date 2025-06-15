#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

int imageInitError(cv::Mat& img, int argc, char** argv)
{
  if ( argc != 2 )
    {
      printf("usage: .out <Image_Path>\n");
      return -1;
    }
    
  if ( !img.data )
    {
      printf("No image data \n");
      return -1;
    }
  return 0;
}  

void imagePrint(cv::Mat& img)
{
  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image", img);
  cv::waitKey(0);
}

void sortLinearBGR (std::vector<cv::Vec3b>& pixels)
{
      std::sort(pixels.begin(), pixels.end(),
          [](const cv::Vec3b& a, const cv::Vec3b& b) {
              return a[0] < b[0];  // sort by blue
          });
    
    std::sort(pixels.begin(), pixels.end(),
          [](const cv::Vec3b& a, const cv::Vec3b& b) {
              return a[1] < b[1];  // sort by green
          });
    
    std::sort(pixels.begin(), pixels.end(),
          [](const cv::Vec3b& a, const cv::Vec3b& b) {
              return a[2] < b[2];  // sort by red
          });
}

void sortBrightness(std::vector<cv::Vec3b>& pixels)
{
  std::sort(pixels.begin(), pixels.end(),
            [](const cv::Vec3b& a, const cv::Vec3b& b) {
                return (a[0] + a[1] + a[2]) < (b[0] + b[1] + b[2]);
            });
}

void sortByColumnCPU(cv::Mat& img)
{
  #pragma omp parallel for
  for(int j = 0; j<img.cols; ++j)
  {    
    std::vector<cv::Vec3b> column;
    for (int i = 0; i< img.rows; ++i)
    {
      column.push_back(img.at<cv::Vec3b>(i, j));
    }
    
    sortBrightness(column);

    for (int i = 0; i< img.rows; ++i)
    {
      img.at<cv::Vec3b>(i, j) = column[i];
    }
  }  
}

void randomSortCPU(cv::Mat& img, float relEntropy)
{
  cv::RNG rng;
  
  int imgArea = img.cols * img.rows;
  int entropy = static_cast<int>(imgArea * relEntropy);
  
  std::vector<cv::Vec3b> randPixels;
  std::vector<cv::Point> randPos;
  
  for (int i = 0; i < entropy; ++i)
  {
    int row = rng.uniform(0, img.rows);
    int col = rng.uniform(0, img.cols);
    randPos.push_back(cv::Point(col,row));
    randPixels.push_back(img.at<cv::Vec3b>(row, col));
  }
  
  sortBrightness(randPixels);
  
  #pragma omp parallel for
  for (int i = 0; i < entropy; ++i)
  {
    img.at<cv::Vec3b>(randPos[i].y, randPos[i].x) = randPixels[i];
  }

}