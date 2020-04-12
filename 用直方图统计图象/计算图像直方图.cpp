/*
计算一维直方图
Author：qingchen—bi
Date：2020.4.12
*/
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;

class Histogram1D{
private:
    int histSize[1];
    float hranges[2];//
    const float* ranges[1];//值范围的指针
    int  channels[1];//要检查的通道数量
public:

    Histogram1D(){//析构函数，初始化私有变量
        histSize[0] = 256;//盒子数量256个
        hranges[0] = 0.0;//像素值范围
        hranges[1] = 256.0;//
        ranges[0] = hranges;//
        channels[0] = 0;//先关注通道0
    }

    cv::Mat getHistogram(const cv::Mat &image){//计算一维直方图
        cv::Mat hist;
        //计算直方图
        cv::calcHist(&image,
                     1, //仅为一个图像的直方图
                     channels, //使用的通道
                     cv::Mat(),//不使用掩码
                     hist, //作为结果的直方图
                     1, //这是一维的直方图
                     histSize, //箱子数量
                     ranges);//像素值范围
        return hist;
    }
    //计算一维直方图，并返回他的图像
    cv::Mat getHistogramImage(const cv::Mat &image,
                              int zoom =1){
        cv::Mat hist = getHistogram(image);//计算一维直方图
        return getImageOfHistogram(hist, zoom);//画直方图
    }
    //创建表示一个直方图的图像（静态方法）
    static cv::Mat getImageOfHistogram(const cv::Mat &hist, int zoom){
        double maxVal = 0;
        double minVal = 0;
        cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);//计算出最大最小像素值
        int histSize = hist.rows;//直方图的大小
        cv::Mat histImg(histSize*zoom,//用于显示直方图的图像
                        histSize*zoom, CV_8U, cv::Scalar(255));

        int hpt = static_cast<int>(0.9*histSize);

        for(int h = 0;h<histSize;h++){

            float binVal = hist.at<float>(h);
            if(binVal > 0){
                int intensity = static_cast<int>(binVal*hpt / maxVal);
                cv::line(histImg, cv::Point(h*zoom, histSize*zoom),
                         cv::Point(h*zoom, (histSize - intensity)*zoom),
                         cv::Scalar(0), zoom);
            }
        }
        return histImg;
    }
};
/*这里给出了计算彩色图像直方图的方法，但没有完全实现。
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
    //计算一维直方图，并返回他的图像
    cv::Mat getHistogramImage(const cv::Mat &image,
                              int zoom =1){
        cv::Mat hist = getHistogram(image);//计算一维直方图
        return getImageOfHistogram(hist, zoom);//画直方图
    }
    //创建表示一个直方图的图像（静态方法）
    static cv::Mat getImageOfHistogram(const cv::Mat &hist, int zoom){
        double maxVal = 0;
        double minVal = 0;
        cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);//计算出最大最小像素值
        int histSize = hist.rows;//直方图的大小
        cv::Mat histImg(histSize*zoom,//用于显示直方图的图像
                        histSize*zoom, CV_8U, cv::Scalar(255));

        int hpt = static_cast<int>(0.9*histSize);

        for(int h = 0;h<histSize;h++){

            float binVal = hist.at<float>(h);
            if(binVal > 0){
                int intensity = static_cast<int>(binVal*hpt / maxVal);
                cv::line(histImg, cv::Point(h*zoom, histSize*zoom),
                         cv::Point(h*zoom, (histSize - intensity)*zoom),
                         cv::Scalar(0), zoom);
            }
        }
        return histImg;
    }


};
*/
int main()
{
    cv::Mat image1 = cv::imread("D:/study/opencvstudy/05/1.jpg");
    Histogram1D h;//直方图对象
    cv::Mat histo = h.getHistogram(image1);//得到直方图结果
//    for(int i=0; i<100; i++){
//        std::cout << "value" << i << "=" <<
//                     histo.at<float>(i) << endl;
//    }

    cv::namedWindow("result");
    cv::imshow("result",
              h.getHistogramImage(image1));
    cv::waitKey(0);

    return 0;
}
