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
    double minHue, double maxHue,//色调区间
    double minSat, double maxSat,//饱和度区间
    Mat& mask){//掩码,可以理解为输出的图像，用来与原图进行位运算，筛选出皮肤
    //转换
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);//分割
    //色调掩码
    cv::Mat mask1;//小于max
    cv::threshold(channels[0], mask1, maxHue, 255,
            cv::THRESH_BINARY_INV);
    cv::Mat mask2;//大于min
    cv::threshold(channels[0], mask2, minHue, 255,
            cv::THRESH_BINARY);
    cv::Mat hueMask;
    if(minHue<maxHue)
        hueMask = mask1 & mask2;
    else {
        hueMask = mask1 | mask2;//穿越了0度中轴线
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
                  160, 10,//色调一般是0-360度∠表示，经验告诉我们是320-20度，也就是320 340 360 10 20 这些啊，所以上面的编程是没有问题的minHue为160
                  10, 255,
                  mask);
    cv::Mat detected(image1.size(), CV_8UC3, cv::Scalar(0, 0, 0));
    image1.copyTo(detected, mask);//将mask与image1重叠后，复制给detected，其中mask为0的地方detected（image1）对应的位置为0（黑色）
    namedWindow("result");
    imshow("result", detected);
    waitKey(0);
    return 0;
}
