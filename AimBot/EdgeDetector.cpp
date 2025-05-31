#include "EdgeDetector.h"

cv::Mat EdgeDetector::DetectEdges(const cv::Mat& grayFrame)
{
    cv::Mat edges;
    cv::Canny(grayFrame, edges, 100, 200);
    return edges;
}
