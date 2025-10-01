#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

namespace pixSort
{
   void brightnessWithThreshold(std::vector<cv::Vec3b>& pixels, float threshold)
   {
       std::sort(pixels.begin(), pixels.end(),[threshold](const cv::Vec3b& a, const cv::Vec3b& b)
       {
          int brightnessA = a[0] + a[1] + a[2];
          int brightnessB = b[0] + b[1] + b[2];
          // Only sort pixels with brightness above threshold
          if (brightnessA < threshold && brightnessB < threshold)
             return false; // don't reorder both dim pixels
          if (brightnessA < threshold)
             return false; // put dimmer pixel later
          if (brightnessB < threshold)
             return true;  // put brighter pixel first
          return brightnessA < brightnessB;
        });
    }  
}

void sortByColumnThresholdCPU(cv::Mat& img, float threshold)
{
  for (size_t j = 0; j < img.cols; ++j)
  {    
    std::vector<cv::Vec3b> column;
    for (size_t i = 0; i< img.rows; ++i)
    {
      column.push_back(img.at<cv::Vec3b>(i, j));
    }
  
    pixSort::brightnessWithThreshold(column, threshold);
  
    for (size_t i = 0; i < img.rows; ++i)
    {
      img.at<cv::Vec3b>(i, j) = column[i];
    }
  }  
}

void sortByRowThresholdCPU(cv::Mat& img, float threshold)
{
  for (size_t i = 0; i < img.rows; ++i)
  {    
    std::vector<cv::Vec3b> row;
    for (size_t j = 0; j < img.cols; ++j)
    {
      row.push_back(img.at<cv::Vec3b>(i, j));
    }
    
    pixSort::brightnessWithThreshold(row, threshold);
    
    for (size_t j = 0; j < img.cols; ++j)
    {
      img.at<cv::Vec3b>(i, j) = row[j];
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
  
  pixSort::brightnessWithThreshold(randPixels, 0); // same as sorting with no threshold
  
  for (size_t i = 0; i < entropy; ++i)
  {
    img.at<cv::Vec3b>(randPos[i].y, randPos[i].x) = randPixels[i];
  }
}

void imagePrint(cv::Mat& img)
{
  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image", img);
  cv::waitKey(0);
}
