#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

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
    //应用查找表修改图像
    Mat apppliLookUp(const Mat& image,const Mat& lookup){
        Mat result;
        LUT(image, lookup,result);
        return result;
    }
    //伸展直方图
    Mat stretch(const Mat& image,int minVal = 0){
        Mat hist = getHistogram(image);//计算直方图
        //得到直方图的左边限值
        int imin = 0;
        for(;imin < histSize[0];imin++){
            if(hist.at<float>(imin) > minVal)//hist数组是一个256长度的,储存了每个像素的个数,这里的意思是左边开始,像素个数小于minval的忽略
                break;
        }
        int imax = histSize[0] - 1;
        for(;imax >= 0;imax--){
            if(hist.at<float>(imax) > minVal)
                break;
        }

        int dim(256);//创建查找表
        Mat lookup(1,
                   &dim,
                   CV_8U);
        for(int i=0;i<256;i++){
            if(i < imin)lookup.at<uchar>(i)=0;
            else if(i>imax)lookup.at<uchar>(i)=255;
            else {
                lookup.at<uchar>(i)=
                        cvRound(255*(i-imin)/(imax-imin));//线性映射

            }
        }
        Mat result;
       result= apppliLookUp(image, lookup);
       return result;
    }
};
int main()
{
    cv::Mat image1 = cv::imread("D:/study/opencvstudy/05/1.jpg",0);
    Mat result, result1;
    Histogram1D h;
    //创建查找表
    int dim(256);
    Mat lut(1,
            &dim,
            CV_8U);
    for(int i= 0; i< 256; i++){
        lut.at<uchar>(i) = 256 - i;
    }

    result = h.apppliLookUp(image1, lut);//查找表修改后的图像结果

    result1 = h.stretch(image1, 0.01f);

    Mat hist = h.getHistogram(image1);
    double grayMaxValue = 0;
        double grayMinValue = 0;
        cv::minMaxLoc(hist, &grayMinValue, &grayMaxValue, NULL, NULL);
        //输出image1直方图的最大值和最小值
        std::cout  << grayMinValue << std::endl;
        std::cout  << grayMaxValue << std::endl;

    Mat hist1 = h.getHistogram(result1);
    double grayMaxValue1 = 0;
        double grayMinValue1 = 0;
        cv::minMaxLoc(hist, &grayMinValue1, &grayMaxValue1, NULL, NULL);
        //输出result1直方图的最大值和最小值
        std::cout  << grayMinValue1 << std::endl;
        std::cout  << grayMaxValue1 << std::endl;

    cv::namedWindow("image1");
    cv::imshow("image1",
              image1);

    cv::namedWindow("result1");
    cv::imshow("result1",
              result1);

    cv::namedWindow("Hresult1");
    cv::imshow("Hresult1",
              h.getHistogramImage(result1));

    cv::namedWindow("Himage1");
    cv::imshow("Himage1",
               h.getHistogramImage(image1));

    cv::namedWindow("Hresult");
    cv::imshow("Hresult",
              h.getHistogramImage(result));

    cv::waitKey(0);
    return 0;
}
