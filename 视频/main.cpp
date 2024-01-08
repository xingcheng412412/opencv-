//视频
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    string path ="/home/sunwei/Opencv/opencv1/1c6e8191c93254d054f0f800ba9dd55e.mp4";
    VideoCapture cap(path); //视频捕捉对象
    Mat img;
    while(true){
        cap.read(img);
        if(!img.empty()){
            imshow("Image",img);
        }
        waitKey(1);
    }
    return 0;
}
