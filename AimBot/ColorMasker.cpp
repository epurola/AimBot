#include "ColorMasker.h"

cv::Mat ColorMasker::MaskRed(const cv::Mat& frame)
{
	cv::Mat hsv;
	ConvertToHSV(frame, hsv);

	cv::Mat mask1, mask2, colorMask;
	cv::inRange(hsv, cv::Scalar(0, 130, 130), cv::Scalar(10, 255, 255), mask1);
	cv::inRange(hsv, cv::Scalar(160, 130,130 ), cv::Scalar(179, 255, 255), mask2);
	cv::bitwise_or(mask1, mask2, colorMask);

	RefineMask(colorMask);

	return colorMask;
}

cv::Mat ColorMasker::MaskPurple(const cv::Mat& frame)
{
	cv::Mat hsv; 
	ConvertToHSV(frame, hsv);

	cv::Mat colorMask;
	cv::inRange(hsv, cv::Scalar(125, 100, 100), cv::Scalar(155, 255, 255), colorMask); //Purple

	RefineMask(colorMask);

	return colorMask;
}

void ColorMasker::ConvertToHSV(const cv::Mat& frame, cv::Mat& hsv)
{
	cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
}

void ColorMasker::RefineMask(cv::Mat& colorMask)
{
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9));
	cv::morphologyEx(colorMask, colorMask, cv::MORPH_CLOSE, kernel, cv::Point(-1, -1), 3);
	cv::dilate(colorMask, colorMask, kernel, cv::Point(-1, -1), 3);

	//blur and threshold to smooth outline shape
	cv::GaussianBlur(colorMask, colorMask, cv::Size(5, 5), 0);
	cv::threshold(colorMask, colorMask, 127, 255, cv::THRESH_BINARY);
}
