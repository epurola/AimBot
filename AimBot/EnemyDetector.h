#pragma once
#include "ColorMasker.h"
#include "ContourAnalyzer.h"
#include "EdgeDetector.h"
#include <memory>

class EnemyDetector 
{
public:
	EnemyDetector();

	cv::Rect detectEnemy(const cv::Mat& frame);

private:
	std::unique_ptr<ColorMasker> colorMasker;
	std::unique_ptr<EdgeDetector> edgeDetector;
	std::unique_ptr<ContourAnalyzer>  contourAnalyzer;

};