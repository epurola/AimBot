#pragma once
#include <opencv2/opencv.hpp>

class ColorMasker 
{
public:
	ColorMasker() = default;

public:
	cv::Mat MaskRed(const cv::Mat& frame);
	cv::Mat MaskPurple(const cv::Mat& frame);

private:
	void ConvertToHSV(const cv::Mat& frame, cv::Mat& hsv);
	void RefineMask(cv::Mat& ColorMask);

private:
	int morphSize = 9;
    int morphIter = 3;
    int blurSize = 5;
};