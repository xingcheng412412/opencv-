#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

cv::Mat removeApple(const cv::Mat& image)
{
    // 将图像从 BGR 格式转换为 HSV 格式
    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    // 设置苹果颜色的范围
    cv::Scalar lower_red1(0, 100, 100);
    cv::Scalar upper_red1(10, 255, 255);
    cv::Scalar lower_red2(170, 100, 100);
    cv::Scalar upper_red2(179, 255, 255);

    // 对图像进行阈值分割
    cv::Mat mask,mask1,mask2;
    cv::inRange(hsvImage, lower_red1, upper_red1, mask1);
    cv::inRange(hsvImage, lower_red2, upper_red2, mask2);
    cv::bitwise_or(mask1,mask2,mask);

    //对掩膜应用形态学操作
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    // 对原始图像和掩码进行按位与操作
    cv::Mat result;
    cv::bitwise_and(image, image, result, mask);

    return result;
}

int main()
{
    cv::Mat image = cv::imread("/home/sunwei/Opencv/扣除苹果/12345.jpg");

    // 调用函数扣除苹果
    cv::Mat result = removeApple(image);

    // 显示结果
    cv::imshow("Result", result);
    cv::waitKey(0);

    return 0;
}