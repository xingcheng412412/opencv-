#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//按照要求进行二值化
inline Mat cut_value_8UC1(cv::Mat tmp, int thres_, int type) {
    cv::Mat result;
    if (type == 0) {
        cv::threshold(tmp, result, thres_, 255, cv::THRESH_BINARY);//大于阈值返回 最大值 255 小于返回0
    }
    else {
        cv::threshold(tmp, result, thres_, 255, cv::THRESH_BINARY_INV);//小于等于阈值返回最大值 255 
    }
    return result;
}
//将im中小于等于minv的部分和大于maxv的部分置0
inline Mat get_dst_8UC1(cv::Mat im, int minv = 0, int maxv = 255, bool only_mask=false) {
    //将大于maxv的部分置255
    cv::Mat mat_maxout_mask = cut_value_8UC1(im, maxv, 0);
    //将小于minv的部分置255
    cv::Mat mat_minout_mask = cut_value_8UC1(im, minv, 1);
    //值域外的mask
    cv::Mat all_out_mask = mat_maxout_mask + mat_minout_mask;
    if (only_mask) {
        return all_out_mask;
    }
    else {
        //值域内的mask
        cv::Mat all_mask = (255 - all_out_mask) / 255;
        //目标数据   0*n=0 1*n=1
        cv::Mat dst_data = all_mask.mul(im);
        return dst_data;
    }
}

Mat findMaxArea2(Mat bin_mat) {
    Mat borad = Mat::zeros(bin_mat.size(), CV_8UC1);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(bin_mat, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
    int max_aero = 0;
    int max_i = 0;
    //遍历所有轮廓，获取最大连通域的面积和下标
    for (int i = 0; i < (int)contours.size(); i++)
    {
        int aero = contourArea(contours[i]);//计算轮廓面积
        if (max_aero < aero) {
            max_aero = aero;
            max_i = i;
        }
    }
    drawContours(borad, contours, max_i, Scalar(255, 255, 255), -1, 8, hierarchy);
    cout << max_i << "  aero:" << max_aero << endl;
    return borad;
}


int main()
{
    string path = "/home/sunwei/Opencv/车牌检测/"  ;
    Mat srcImage = imread(path + "12345.jpg");
    resize(srcImage, srcImage, { 600,800 });
    //将图像的颜色空间从BGR转换为HSV
    cv::Mat hsv;//H:0—180, S:0—255, V:0—255
    cv::cvtColor(srcImage, hsv, cv::COLOR_BGR2HSV);
    vector<cv::Mat> mv;
    cv::split(hsv, mv);
    //hsv空间颜色范围参考   
    Mat yellow_mask = get_dst_8UC1(mv[0], 26, 34);
    Mat green_mask = get_dst_8UC1(mv[0], 35, 80);
    Mat blue_mask = get_dst_8UC1(mv[0], 100, 124);
    Mat mask = yellow_mask + green_mask + blue_mask;
    Mat s_mask = get_dst_8UC1(mv[1], 43, 256);
    Mat v_mask = get_dst_8UC1(mv[2], 46, 256);

    s_mask.convertTo(s_mask, CV_32FC1);
    v_mask.convertTo(v_mask, CV_32FC1);
    Mat sv_mask = s_mask.mul(v_mask/255.0) / 255;
    sv_mask = (sv_mask*255)*2;//为了将值域从0,1转换为0,255
    sv_mask.convertTo(sv_mask, CV_8UC1);

    Mat  bin_mat, final_mask;
    adaptiveThreshold(sv_mask, bin_mat, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 35, -30);
    final_mask = mask.mul(bin_mat);
    imwrite(path + "_sv.jpg", final_mask );
    //车牌区域优化1
    Mat  blackhat_mat,dilate_mat;
    morphologyEx(final_mask, blackhat_mat, MORPH_BLACKHAT, getStructuringElement(MORPH_RECT, Size(7, 7)));  
    morphologyEx(blackhat_mat, dilate_mat, MORPH_BLACKHAT, getStructuringElement(MORPH_RECT, Size(17, 17)));


    //车牌区域提取 （此时车牌区域已经是dilate_mat图像中的主要区域）
    Mat  dilate_close,dilate_open;
    //先使车牌中孔洞和缺口区域闭合
    morphologyEx(dilate_mat, dilate_close, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(13, 7)));
    //消除小面积区域,使车牌区域与其他噪声断开
    //此时在大部分图片中车牌区域已经被提取出来了
    morphologyEx(dilate_close, dilate_open, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(51, 21)));
    //---车牌区域精提取---
    //找到最大的连通域
    Mat max_aero= findMaxArea2(dilate_open);
    //确保最大的连通域能完全覆盖车牌区域
    Mat max_aero_dilate, bin_mat_dilate;
    morphologyEx(max_aero, max_aero_dilate, MORPH_DILATE, getStructuringElement(MORPH_RECT, Size(31, 23)));
    //对bin_mat进行进行闭运算，使车牌区域完成闭合
    morphologyEx(bin_mat, bin_mat_dilate, MORPH_CLOSE,
    getStructuringElement(MORPH_RECT, Size(17, 23)));
    Mat final_aero,final_mat;
    bitwise_and(max_aero_dilate, bin_mat_dilate, final_aero);
    srcImage.copyTo(final_mat, final_aero);
    //对bin_mat进行
    imwrite(path + "_max_aero_dilate.jpg", max_aero_dilate);
    imwrite(path + "_bin_mat_dilate.jpg", bin_mat_dilate);
    imwrite(path + "_final_aero.jpg", final_aero);
    imwrite(path + "_final_img.jpg", final_mat);
    Rect bound=boundingRect(final_aero);
    Mat only_car_num2 = final_mat(bound);
    //对bin_mat进行
    imwrite(path + "_carNum.jpg", only_car_num2);


    return 0;

}