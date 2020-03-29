/*
皮肤检测算法
Author：qingchen—bi
Date：2020.3.29
*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void detectHScolor(const Mat& image,
    double minHue, double maxHue,
    double minSat, double maxSat,
    Mat& mask){
    //转换
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    //色调掩码
    cv::Mat mask1;
    cv::threshold(channels[0], mask1, maxHue, 255,
            cv::THRESH_BINARY_INV);
    cv::Mat mask2;
    cv::threshold(channels[0], mask2, minHue, 255,
            cv::THRESH_BINARY);
    cv::Mat hueMask;
    if(minHue<maxHue)
        hueMask = mask1 & mask2;
    else {
        hueMask = mask1 | mask2;
    }
    //饱和度掩码
    cv::threshold(channels[0], mask1, maxSat, 255,
            cv::THRESH_BINARY_INV);
    cv::threshold(channels[0], mask2, minSat, 255,
            cv::THRESH_BINARY);
    cv::Mat satMask;  //饱和度掩码
    satMask = mask1 & mask2;
    //组合掩码
    mask = hueMask&satMask;
}
int main()
{
    Mat image1 = imread("D:/study/opencvstudy/05/1.jpg");
    Mat mask;
    detectHScolor(image1,
                  120, 10,
                  10, 255,
                  mask);
    cv::Mat detected(image1.size(), CV_8UC3, cv::Scalar(0, 0, 0));
    image1.copyTo(detected, mask);
    namedWindow("result");
    imshow("result", detected);
    waitKey(0);
    return 0;
}
