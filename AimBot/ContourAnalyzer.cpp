#include "ContourAnalyzer.h"

std::vector<cv::Rect> ContourAnalyzer::getBoundingBoxes(const cv::Mat& binaryImage, int minWidth, int minHeight)
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Rect> boxes;

    cv::findContours(binaryImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        cv::Rect box = cv::boundingRect(contour);
        if (box.width >= minWidth && box.height >= minHeight) {
            boxes.push_back(box);
        }
    }

    return boxes;
}
