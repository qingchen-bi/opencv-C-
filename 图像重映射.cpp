/*
实现图像的重映射，生成波浪形的图像
Author：qingchen—bi
Date：2020.3.29
*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

void wave(const Mat &image, Mat &result){//重映射图像，创建波浪效果
    Mat srcX(image.rows, image.cols, CV_32F);//映射参数
    Mat srcY(image.rows, image.cols, CV_32F);
    for(int i=0; i<image.rows; i++){
        for(int j=0; j<image.cols; j++){
            srcX.at<float>(i,j)=j;//X方向的映射，由于是产生波浪，所以不会变化
            srcY.at<float>(i,j)=i+5*sin(j/10);//Y方向的映射，波浪，所以上下在Y方向目标图像与原图像有错位
        }
    }
    remap(image, result, srcX, //srcX.srcY分别表示目标图像中（x，y）在原图像中的位置
          srcY, //Y方向映射
          INTER_LINEAR);//像素插值方法
}

int main()
{
    Mat image1 = imread("D:/study/opencvstudy/04/8.jpg");
    Mat result;
    wave(image1, result);
    namedWindow("Image1");
    imshow("Image1", result);
    waitKey(0);

    return 0;
}
