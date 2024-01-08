#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap(0);
    Mat img;

    CascadeClassifier faceCascade;
    faceCascade.load("/home/sunwei/Opencv/人脸识别/haarcascade_frontalface_default.xml");

    if (faceCascade.empty()) { cout << "XML file not loaded" << endl;}

    while (true){
        cap.read(img);

        vector<Rect> faces;
        faceCascade.detectMultiScale(img, faces, 1.1, 10);

        for (int i = 0; i < (int)faces.size(); i++)
        {
            rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);
        }

        if(!img.empty()){
            imshow("Image",img);
        }
        waitKey(1);
    }

    return 0;
}