#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    string path = "/home/sunwei/Opencv/调整和剪裁/2023-10-03 15-31-17 的屏幕截图.png";
    Mat img = imread(path);
    Mat imgResize , imgCrop;

    cout << img.size() << endl;
    resize(img,imgResize,Size(),0.5,0.5);

    Rect roi(200,100,300,300);
    imgCrop = img(roi);

    if(!img.empty()){
        imshow("Image",img);
    }
    if(!imgResize.empty()){
        imshow("Image",imgResize);
    }
    if(!imgCrop.empty()){
        imshow("Image",imgCrop);
    }
    waitKey(0);
    
    return 0;
}