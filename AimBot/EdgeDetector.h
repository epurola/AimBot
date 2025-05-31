#pragma once
#include <opencv2/opencv.hpp>

class EdgeDetector 
{
public:
	EdgeDetector() = default;
	cv::Mat DetectEdges(const cv::Mat& grayFrame);

private:


};