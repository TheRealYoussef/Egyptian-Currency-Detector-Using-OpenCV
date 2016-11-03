#include "gradientfilter.h"
#include <cmath>
#define PI 3.14159265358979323846

cv::Mat GradientFilter::getNormalizedGradientMagnitude(const cv::Mat& image, int threshold) {
    std::vector<std::vector<double> > magnitude(image.rows - 2);
    int gx, gy;
    double minimum = DBL_MAX, maximum = DBL_MIN;
    for (int i = 1; i < image.rows - 1; ++i) {
        for (int j = 1; j < image.cols - 1; ++j) {
            gx = image.at<uchar>(i - 1, j + 1) + 2 * image.at<uchar>(i, j + 1) + image.at<uchar>(i + 1, j + 1)- image.at<uchar>(i - 1, j - 1) - 2 * image.at<uchar>(i, j - 1) - image.at<uchar>(i + 1, j - 1);
            gy = image.at<uchar>(i + 1, j - 1) + 2 * image.at<uchar>(i + 1, j) + image.at<uchar>(i + 1, j + 1)- image.at<uchar>(i - 1, j - 1) - 2 * image.at<uchar>(i - 1, j) - image.at<uchar>(i - 1, j + 1);
            magnitude[i - 1].push_back(std::sqrt(gx * gx + gy * gy));
            minimum = std::min(minimum, magnitude[i - 1][j - 1]);
            maximum = std::max(maximum, magnitude[i - 1][j - 1]);
        }
    }
    return normalize(magnitude, minimum, maximum, image.rows - 2, image.cols - 2, threshold);

}

cv::Mat GradientFilter::normalize(const std::vector<std::vector<double> >& magnitude, double minimum, double maximum, int rows, int cols, double threshold) {
    cv::Mat normalizedGradientMagnitude = cv::Mat(rows, cols, CV_8UC1, cv::Scalar(0));
    int normalValue;
    double maxMinusMin, scale;
    maxMinusMin = maximum - minimum;
    for (int i = 0; i < normalizedGradientMagnitude.rows; ++i) {
        for (int j = 1; j < normalizedGradientMagnitude.cols; ++j) {
            scale = (magnitude[i][j] - minimum) * 1.0 / maxMinusMin;
            normalValue = (int)(255 * scale);
            normalizedGradientMagnitude.at<uchar>(i, j) = normalValue >= threshold ? 255 : 0;
        }
    }
    return normalizedGradientMagnitude;
}
