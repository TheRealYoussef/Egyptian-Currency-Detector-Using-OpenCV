#ifndef GRADIENTFILTER_H
#define GRADIENTFILTER_H

#include <opencv2/opencv.hpp>
#include <vector>

class GradientFilter
{
public:

    /*
     * Apply Sobel operators and calculate magnitude and direction. Then normalizes magnitude
     * and changes any value >= threshold to 255 and any value < threshold to 0
    */
    static cv::Mat getNormalizedGradientMagnitude(const cv::Mat& image, int threshold);

    static cv::Mat normalize(const std::vector<std::vector<double> >&, double minimum, double maximum, int rows, int cols, double threshold);
};

#endif // GRADIENTFILTER_H
