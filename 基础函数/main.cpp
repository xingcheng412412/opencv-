//基础函数
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    string path = "/home/sunwei/Opencv/基础函数/_-1731416672__08646f0b503fc9682b258580f28f7e58_1031088803_IMG_20230930_160522_0_xg_0.jpg";
    Mat img = imread(path);
    Mat imgGray,imgBlur,imgCanny,imgDil,imgErode;

    cvtColor(img,imgGray,COLOR_BGR2GRAY);   //灰度化
    GaussianBlur(img,imgBlur,Size(3,3),3,0);   //高斯模糊
    Canny(imgBlur,imgCanny,25,75);           //边缘检测

    Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3));
    dilate(imgCanny,imgDil,kernel);
    erode(imgDil,imgErode,kernel);

    if(!img.empty()){
        imshow("Image",img);
    }
    if(!imgGray.empty()){
        imshow("ImageGray",imgGray);
    }
    if(!imgBlur.empty()){
        imshow("ImageBlur",imgBlur);
    }
    if(!imgCanny.empty()){
        imshow("ImageCanny",imgCanny);
    }
    if(!imgDil.empty()){
        imshow("ImageDil",imgDil);
    }
    if(!imgErode.empty()){
        imshow("ImageErode",imgErode);
    }
    waitKey(0);

    return 0;
}