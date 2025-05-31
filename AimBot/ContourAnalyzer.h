#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class ContourAnalyzer 
{
public:
    ContourAnalyzer() = default;
    std::vector<cv::Rect> getBoundingBoxes(const cv::Mat& binaryImage, int minWidth = 20, int minHeight = 20);
};