#pragma once
#include "Windows.h"
#include <opencv2/opencv.hpp>

class ScreenRecorder {
public:
	ScreenRecorder(int width, int height);
	cv::Mat CaptureScreen();

	int GetCenterX() const { return centerX; }
	int GetCenterY() const { return centerY; }
	
private:
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	int centerX = 0;
	int centerY = 0;
};