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
    static void getNormalizedGradientMagnitudeAndDirection(const cv::Mat& image, int threshold, cv::Mat& normalizedGradientMagnitude, std::vector<std::vector<double> >& gradientDirection);
};

#endif // GRADIENTFILTER_H
