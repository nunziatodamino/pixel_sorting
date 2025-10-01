#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

namespace pixSort
{
  void linearBGR (std::vector<cv::Vec3b>& pixels)
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

  void brightness(std::vector<cv::Vec3b>& pixels)
  {
    std::sort(pixels.begin(), pixels.end(),
              [](const cv::Vec3b& a, const cv::Vec3b& b) {
                  return (a[0] + a[1] + a[2]) < (b[0] + b[1] + b[2]);
              });
  }
  
void brightnessWithThreshold(std::vector<cv::Vec3b>& pixels, float threshold)
{
    std::sort(pixels.begin(), pixels.end(),
              [threshold](const cv::Vec3b& a, const cv::Vec3b& b)
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

void imagePrint(cv::Mat& img)
{
  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image", img);
  cv::waitKey(0);
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
    
    pixSort::brightness(column);

    for (int i = 0; i< img.rows; ++i)
    {
      img.at<cv::Vec3b>(i, j) = column[i];
    }
  }  
}

void blueChannel(cv::Mat& img)
{
  std::vector<cv::Mat> channels;
  cv::split(img, channels);
  channels[1].setTo(0);
  channels[2].setTo(0);
  cv::merge(channels, img);
}


void sortByColumnThresholdCPU(cv::Mat& img, float threshold)
{
  for(int j = 0; j<img.cols; ++j)
  {    
    std::vector<cv::Vec3b> column;
    for (int i = 0; i< img.rows; ++i)
    {
      column.push_back(img.at<cv::Vec3b>(i, j));
    }
    
    pixSort::brightnessWithThreshold(column, threshold);
   
    #pragma omp parallel for
    for (int i = 0; i< img.rows; ++i)
    {
      img.at<cv::Vec3b>(i, j) = column[i];
    }
  }  
}

void sortByRowThresholdCPU(cv::Mat& img, float threshold)
{
  for(int i = 0; i<img.rows; ++i)
  {    
    std::vector<cv::Vec3b> row;
    for (int j = 0; j< img.cols; ++j)
    {
      row.push_back(img.at<cv::Vec3b>(i, j));
    }
    
    pixSort::brightnessWithThreshold(row, threshold);
    
    #pragma omp parallel for
    for (int j = 0; j< img.cols; ++j)
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
  
  pixSort::brightness(randPixels);
  
  #pragma omp parallel for
  for (int i = 0; i < entropy; ++i)
  {
    img.at<cv::Vec3b>(randPos[i].y, randPos[i].x) = randPixels[i];
  }

}