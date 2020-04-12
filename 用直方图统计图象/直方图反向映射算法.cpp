/*
利用直方图反向映射算法查找彩色图像特定内容
Author：qingchen-bi
Date:2020.4.12
*/
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class ContentFinder{
private:
    //直方图参数
    float hranges[2];
    const float* ranges[3];
    int channels[3];

    float threshold;//判断阈值
    Mat histogram;//输入直方图
public:
    ContentFinder():threshold(0.1f){
        ranges[0] = hranges;
        ranges[1] = hranges;
        ranges[2] = hranges;
    }

    void setThreshold(float t){
        threshold = t;
    }
    float getThreshold(){
        return threshold;
    }
    void setHistogram(const cv::Mat &h){
        histogram = h;
        cv::normalize(histogram,histogram,1.0);
    }
    Mat find(Mat &image){
        Mat result;
        hranges[0] = 0.0;
        hranges[1] = 256.0;
        channels[0] = 0;
        channels[1] = 1;
        channels[2] = 2;

        return find(image, hranges[0], hranges[1], channels);
    }
    Mat find(const Mat &image,
             float minValue,
             float maxValue,
             int * channels) {
        Mat result;
        hranges[0] = minValue;
        hranges[1] = maxValue;

        for (int i = 0;i<histogram.dims;i++)
            this->channels[i] = channels[i];
        cv::calcBackProject(&image,
                        1,
                        channels,
                        histogram,
                        result,
                        ranges,
                        255.0
       );
        if (threshold>0.0)// 设置的阈值>0时，才进行阈值分割
            cv::threshold(result, result, 255.0*threshold, 255.0, cv::THRESH_BINARY);
        return result;
    }

};
class ColorHistogram
{
private:
    int histSize[3];//每个维度的大小
    float hranges[2];//像素值范围
    const float* ranges[3];
    int channels[3];//通道数
public:
    ColorHistogram() {
        histSize[0]=histSize[2]=histSize[1]=256;
        hranges[0]=0.0;
        hranges[1]=256.0;
        ranges[0]=hranges;
        ranges[1]=hranges;
        ranges[2]=hranges;
        channels[0]=0;
        channels[1]=1;
        channels[2]=2;
    }
    //计算出直方图，返回的是三维的cv::Mat实例
    cv::Mat getHistogram(const cv::Mat &image){
        cv::Mat hist;
        hranges[0]=0.0;
        hranges[1]=256.0;
        channels[0]=0;
        channels[1]=1;
        channels[2]=2;
        //计算直方图
        cv::calcHist(&image,
                     1,
                     channels,
                     cv::Mat(),
                     hist,
                     3,
                     histSize,
                     ranges);
        return hist;
    }
    //计算出直方图，由于上面的方法计算的太多，用稀疏的（省掉比较少的像素值）
    cv::SparseMat getSparseHistogram(const cv::Mat &image){
        cv::SparseMat hist(3,//维数
                           histSize,//每个维度的大小
                           CV_32F);
        //BGR颜色的直方图
        hranges[0]=0.0;
        hranges[1]=256.0;
        channels[0]=0;
        channels[1]=1;
        channels[2]=2;
        //计算直方图
        cv::calcHist(&image, 1, channels, cv::Mat(), hist, 3, histSize, ranges);
        return hist;
    }
};
int main()
{
    cv::Mat image1 = cv::imread("D:/study/opencvstudy/08/1.png");
    ColorHistogram h;
    Mat imageROI = image1(Rect(470,160,20,20));
    Mat hist = h.getHistogram(imageROI);
    ContentFinder finder;
    finder.setHistogram(hist);
    finder.setThreshold(0.005f);
    Mat result = finder.find(image1);
    cv::namedWindow("result");
    cv::imshow("result",result);
    cv::waitKey(0);

    return 0;
}
