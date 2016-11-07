#include "mainwindow.h"
#include <QApplication>
#include <qdebug.h>
#include <map>
#include <opencv2/opencv.hpp>
#include "line.h"
#include "circle.h"

#define PI 3.14159265358979323846

void getIntersecionPoints(int angle1, int ro1, int angle2, int ro2, cv::Point& point) {
    if (angle1 == 0) {
        point.x = ro1 / std::cos(angle1 * PI / 180);
        point.y = (ro2 - point.x * std::cos(angle2 * PI / 180)) / std::sin(angle2 * PI / 180);
    }
    else if (angle1 == 90) {
        point.y = ro1 / std::sin(angle1 * PI / 180);
        point.x = (ro2 - point.y * std::sin(angle2 * PI / 180)) / std::cos(angle2 * PI / 180);
    }
    else if (angle2 == 0) {
        point.x = ro2 / std::cos(angle2 * PI / 180);
        point.y = (ro1 - point.x * std::cos(angle1 * PI / 180)) / std::sin(angle1 * PI / 180);
    }
    else if (angle2 == 90) {
        point.y = ro2 / std::sin(angle2 * PI / 180);
        point.x = (ro1 - point.y * std::sin(angle1 * PI / 180)) / std::cos(angle1 * PI / 180);
    }
    else {
        double sinAngle1 = std::sin(angle1 * PI / 180);
        double sinAngle2 = std::sin(angle2 * PI / 180);
        double cosAngle1 = std::cos(angle1 * PI / 180);
        double cosAngle2 = std::cos(angle2 * PI / 180);
        point.x = (ro1 * sinAngle2 - ro2 * sinAngle1) / (cosAngle1 * sinAngle2 - cosAngle2 * sinAngle1);
        point.y = (ro1 - point.x * cosAngle1) / sinAngle1;
    }
}

void accumulateLines(const cv::Mat& dst, std::vector<std::vector<int> >& lineAccumulator) {
    for (int i = 0; i < dst.rows; ++i) {
        for (int j = 0; j < dst.cols; ++j) {
            if (dst.at<uchar>(i, j) == 255) {
                for (int a = 0; a < 180; ++a) {
                    ++lineAccumulator[a][std::round(j * std::cos(a * PI / 180) + i * std::sin(a * PI / 180)) + dst.rows + dst.cols - 2];
                }
            }
        }
    }
}

void getAllLines(const std::vector<std::vector<int> >& lineAccumulator, std::vector<std::pair<int, Line> >& allLines, int threshold, int rows, int cols) {
    for (int i = 0; i < (int)lineAccumulator.size(); ++i) {
        for (int j = 0; j < (int)lineAccumulator[i].size(); ++j) {
            if (lineAccumulator[i][j] >= threshold) {
                allLines.push_back(std::make_pair(lineAccumulator[i][j], Line(i, j - rows - cols + 2)));
            }
        }
    }
}

void getBestLines(const std::vector<std::pair<int, Line> >& allLines, std::vector<Line>& lines) {
    for (int i = allLines.size() - 1; i >= 0; --i) {
        bool equal = false;
        for (int j = 0; j < (int)lines.size(); ++j) {
            if (allLines[i].second == lines[j]) {
                equal = true;
                break;
            }
        }
        if (!equal)
            lines.push_back(allLines[i].second);
    }
}

void drawRectangles(const std::vector<Line>& lines, cv::Mat& image) {
    std::vector<bool> used(lines.size(), false);

    for (int i = 0; i < (int)lines.size(); i += 2) {
        if (!used[i]) {
            for (int j = i + 2; j < (int)lines.size(); j += 2) {
                if (std::abs(lines[i].angle + 90 - lines[j].angle) <= 2 && !used[j]) {
                    cv::Point point1, point2;
                    getIntersecionPoints(lines[i].angle, lines[i].ro, lines[j].angle, lines[j].ro, point1);
                    getIntersecionPoints(lines[i].angle, lines[i].ro, lines[j + 1].angle, lines[j + 1].ro, point2);
                    cv::line(image, point1, point2, cv::Scalar(0, 0, 255), 2);
                    getIntersecionPoints(lines[i + 1].angle, lines[i + 1].ro, lines[j].angle, lines[j].ro, point1);
                    getIntersecionPoints(lines[i + 1].angle, lines[i + 1].ro, lines[j + 1].angle, lines[j + 1].ro, point2);
                    cv::line(image, point1, point2, cv::Scalar(0, 0, 255), 2);
                    getIntersecionPoints(lines[i].angle, lines[i].ro, lines[j].angle, lines[j].ro, point1);
                    getIntersecionPoints(lines[i + 1].angle, lines[i + 1].ro, lines[j].angle, lines[j].ro, point2);
                    cv::line(image, point1, point2, cv::Scalar(0, 0, 255), 2);
                    getIntersecionPoints(lines[i].angle, lines[i].ro, lines[j + 1].angle, lines[j + 1].ro, point1);
                    getIntersecionPoints(lines[i + 1].angle, lines[i + 1].ro, lines[j + 1].angle, lines[j + 1].ro, point2);
                    cv::line(image, point1, point2, cv::Scalar(0, 0, 255), 2);
                    used[i] = used[j] = true;
                    break;
                }
            }
        }
    }
}

void accumulate(int i, int j, int rows, int cols, std::vector<std::vector<int> >& accumulator) {
    if (i >= 0 && i < rows && j >= 0 && j < cols) {
        ++accumulator[i][j];
    }
}

void accumulateCircles(const cv::Mat& dst, std::vector<std::vector<int> >& circleAccumulator, int r) {
    int x, y, err;
    for (int i = 0; i < dst.rows; ++i) {
        for (int j = 0; j < dst.cols; ++j) {
            if (dst.at<uchar>(i, j) == 255) {
                //Midpoint circle algorithm https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
                x = r;
                y = err = 0;
                while (x >= y) {
                    accumulate(i + x, j + y, dst.rows, dst.cols, circleAccumulator);
                    accumulate(i - x, j + y, dst.rows, dst.cols, circleAccumulator);
                    accumulate(i + y, j + x, dst.rows, dst.cols, circleAccumulator);
                    accumulate(i + y, j - x, dst.rows, dst.cols, circleAccumulator);
                    if (y != 0) {
                        accumulate(i - y, j + x, dst.rows, dst.cols, circleAccumulator);
                        accumulate(i - y, j - x, dst.rows, dst.cols, circleAccumulator);
                        accumulate(i + x, j - y, dst.rows, dst.cols, circleAccumulator);
                        accumulate(i - x, j - y, dst.rows, dst.cols, circleAccumulator);
                    }
                    y += 1;
                    err += 1 + 2 * y;
                    if (2 * (err - x) + 1 > 0)
                    {
                        x -= 1;
                        err += 1 - 2 * x;
                    }
                }
            }
        }
    }
}

void getAllCircles(const std::vector<std::vector<int> >& circleAccumulator, std::vector<std::pair<int, Circle> >& allCircles, int threshold, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (circleAccumulator[i][j] >= threshold) {
                allCircles.push_back(std::make_pair(circleAccumulator[i][j], Circle(j, i)));
            }
        }
    }
}

void getBestCircles(const std::vector<std::pair<int, Circle> >& allCircles, std::vector<Circle>& circles) {
    for (int i = allCircles.size() - 1; i >= 0; --i) {
        bool equal = false;
        for (int j = 0; j < (int)circles.size(); ++j) {
            if (allCircles[i].second == circles[j]) {
                equal = true;
                break;
            }
        }
        if (!equal)
            circles.push_back(allCircles[i].second);
    }
}

void drawCircles(const std::vector<Circle>& circles, cv::Mat& image, int r) {
    for (int i = 0; i < (int)circles.size(); ++i) {
        cv::circle(image, cv::Point(circles[i].x, circles[i].y), r, cv::Scalar(0, 0, 255), 2);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    cv::Mat image = cv::imread("C:/Users/Youssef/Desktop/pic17.jpg", CV_LOAD_IMAGE_COLOR);
    cv::Mat imageGrayscale = cv::imread("C:/Users/Youssef/Desktop/pic17.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat dst;

    cv::blur(imageGrayscale, dst, cv::Size(3, 3));

    cv::Canny(dst, dst, 128, 200);

    cv::imshow("Canny", dst);

    std::vector<std::vector<int> > lineAccumulator(180, std::vector<int>((image.rows + image.cols - 2) * 2, 0));

    accumulateLines(dst, lineAccumulator);

    std::vector<std::pair<int, Line> > allLines;

    getAllLines(lineAccumulator, allLines, 150, image.rows, image.cols);

    sort(allLines.begin(), allLines.end());

    std::vector<Line> lines;

    getBestLines(allLines, lines);

    sort(lines.begin(), lines.end());

    //Testing lines
    /*
    for (int i = 0; i < lines.size(); ++i) {
        cv::line(image, cv::Point(0, lines[i].ro / std::sin(lines[i].angle * PI / 180)), cv::Point(image.cols - 1, (-image.cols + 1) * (std::cos(lines[i].angle * PI / 180) / std::sin(lines[i].angle * PI / 180)) + lines[i].ro / std::sin(lines[i].angle * PI / 180)), cv::Scalar(0, 0, 255), 2);
        qDebug() << lines[i].angle << ' ' << lines[i].ro;
        qDebug() << 0 << ' ' << lines[i].ro / std::sin(lines[i].angle * PI / 180) << ' ' << image.cols - 1 << ' ' << (-image.cols + 1) * (std::cos(lines[i].angle * PI / 180) / std::sin(lines[i].angle * PI / 180)) + lines[i].ro / std::sin(lines[i].angle * PI / 180);
    }
    */

    drawRectangles(lines, image);

    std::vector<std::vector<int> > circleAccumulator(dst.rows, std::vector<int>(dst.cols, 0));

    int r = 64;

    accumulateCircles(dst, circleAccumulator, r);

    std::vector<std::pair<int, Circle> > allCircles;

    getAllCircles(circleAccumulator, allCircles, r * 1.53, image.rows, image.cols);

    std::sort(allCircles.begin(), allCircles.end());

    std::vector<Circle> circles;

    getBestCircles(allCircles, circles);

    drawCircles(circles, image, r);

    cv::imshow("Image", image);

    return a.exec();
}
