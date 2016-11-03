#include "gradientfilter.h"
#include <cmath>
#define PI 3.14159265358979323846

void GradientFilter::getNormalizedGradientMagnitudeAndDirection(const cv::Mat& image, int threshold, cv::Mat& normalizedGradientMagnitude, std::vector<std::vector<double> >& gradientDirection) {
    normalizedGradientMagnitude = cv::Mat(image.rows - 2, image.cols - 2, CV_8UC1, cv::Scalar(0));
    gradientDirection.resize(image.rows - 2);
    std::vector<std::vector<int> > magnitude(image.rows - 2);
    int minimum = INT_MAX, maximum = INT_MIN, maxMinusMin, normalValue, gx, gy;
    double scale;
    for (int i = 1; i < image.rows - 1; ++i) {
        for (int j = 1; j < image.cols - 1; ++j) {
            gx = image.at<uchar>(i - 1, j + 1) + 2 * image.at<uchar>(i, j + 1) + image.at<uchar>(i + 1, j + 1)- image.at<uchar>(i - 1, j - 1) - 2 * image.at<uchar>(i, j - 1) - image.at<uchar>(i + 1, j - 1);
            gy = image.at<uchar>(i + 1, j - 1) + 2 * image.at<uchar>(i + 1, j) + image.at<uchar>(i + 1, j + 1)- image.at<uchar>(i - 1, j - 1) - 2 * image.at<uchar>(i - 1, j) - image.at<uchar>(i - 1, j + 1);
            magnitude[i - 1].push_back(std::sqrt(gx * gx + gy * gy));
            gradientDirection[i - 1].push_back(std::atan(gy * 1.0 / gx) * PI / 180.0);
            minimum = std::min(minimum, magnitude[i - 1][j - 1]);
            maximum = std::max(maximum, magnitude[i - 1][j - 1]);
        }
    }
    maxMinusMin = maximum - minimum;
    for (int i = 0; i < normalizedGradientMagnitude.rows; ++i) {
        for (int j = 1; j < normalizedGradientMagnitude.cols; ++j) {
            scale = (magnitude[i][j] - minimum) * 1.0 / maxMinusMin;
            normalValue = 255 * scale;
            normalizedGradientMagnitude.at<uchar>(i, j) = normalValue >= threshold ? 255 : 0;
        }
    }
}
