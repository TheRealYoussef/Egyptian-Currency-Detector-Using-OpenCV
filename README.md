# Egyptian Currency Detector using OpenCV
Qt widgets application with OpenCV library. Obviously requires Qt and OpenCV to be setup for this code to run.

Note that the following algorithm is mentioned without the thresholds because different images
require different thresholds. Attached with this submission are a group of images which work
well with the hard-coded thresholds in the code.

Algorithm:
1. Blur the grayscale image
2. Use Canny Edge Detector
3. For each edge point from Canny, loop from [0 to 179] and add to the accumulator
lineAccumulator[theta][ro]
4. Loop on lineAccumulator and for each point larger than lineThreshold, add the
corresponding line the the vector allLines
5. Loop on the vector allLines and choose the most important lines
6. Draw the rectangles using these lines
7. For each edge point from Canny, add to circleAccumulator using the Midpoint Circle
Algorithm https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
8. Loop on circleAccumulator and for each point larger the circleThreshold, add the
corresponding circle to the vector allCircles
9. Loop on the vector allCircles and choose the most important circles
10. Draw these circles
