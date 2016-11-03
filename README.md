# Egyptian Currency Detector using OpenCV
Qt widgets application with OpenCV library. Obviously requires Qt and OpenCV to be setup for this code to run.

Algorithm:
1- Apply Sobel operator on image grayscale
2- Perform Circular Hough Transform to detect coins and General Hough Transform to detect paper money
3- Use color to approximate value

Notes:
Best results on images with good lighting, good contrast with background, and new currency (color not faded).
