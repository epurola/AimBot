#include "EnemyDetector.h"

EnemyDetector::EnemyDetector()
{
	contourAnalyzer = std::make_unique<ContourAnalyzer>();
	colorMasker = std::make_unique<ColorMasker>();
	edgeDetector = std::make_unique<EdgeDetector>();
}

cv::Rect EnemyDetector::detectEnemy(const cv::Mat& frame)
{
	cv::Mat mask = colorMasker->MaskPurple(frame);

	cv::Mat maskedBGR;
	cv::bitwise_and(frame, frame, maskedBGR, mask);

	cv::Mat gray;
	cv::cvtColor(maskedBGR, gray, cv::COLOR_BGR2GRAY);

	cv::Mat edges = edgeDetector->DetectEdges(gray);

	std::vector<cv::Rect> boxes = contourAnalyzer->getBoundingBoxes(edges);

	cv::Rect largest;
	int maxArea = 0;
	for (const auto& box : boxes) {
		int area = box.width * box.height;
		if (area > maxArea) {
			maxArea = area;
			largest = box;
		}
	}

	return largest;

}
