#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat imgHSV,mask;
int hmin = 0, smin = 110, vmin = 153;
int hmax = 19, smax =240, vmax = 255;
 
int main()
{
    string path = "/home/sunwei/Opencv/颜色检测/123.jpg";
    Mat img = imread(path);
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    namedWindow("Trackbars", (640, 200));
    createTrackbar("Hue Min", "Trackbars", &hmin, 179);
    createTrackbar("Hue Max", "Trackbars", &hmax, 179);
    createTrackbar("Sat Min", "Trackbars", &smin, 255);
    createTrackbar("Sat Max", "Trackbars", &smax, 255);
    createTrackbar("Val Min", "Trackbars", &vmin, 255);
    createTrackbar("Val Max", "Trackbars", &vmax, 2555);

    while(true) {
        Scalar lower(hmin,smin,vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(imgHSV, lower, upper, mask);

        if(!img.empty()){
            imshow("Image", img);
        }
        if(!img.empty()){
            imshow("Image HSV", imgHSV);
        }
        if(!img.empty()){
            imshow("Image Mask", mask);
        }
        waitKey(1);
    }

    return 0;
}