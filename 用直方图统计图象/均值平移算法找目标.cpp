#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
using namespace cv;

class ContentFinder{//直方图反映射查找特定图像区域
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

class ColorHistogram//彩色直方图
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
    Mat getHueHistogram(const cv::Mat &image,
                     int minSaturation = 0){
        Mat hist;
        Mat hsv;
        cvtColor(image,hsv,COLOR_BGR2HSV);
        Mat mask;
        if(minSaturation>0){//屏蔽低饱和度的像素
            std::vector<Mat> v;
            split(hsv, v);
            threshold(v[1], mask, minSaturation, 255,
                    THRESH_BINARY);
        }
        //准备一维直方图的参数
        hranges[0] = 0.0;
        hranges[1] = 180.0;
        channels[0] = 0;
        //计算直方图
        calcHist(&hsv, 1, channels, mask, hist, 1, histSize, ranges);
        return hist;
    }
};
int main()
{
    cv::Mat image1 = cv::imread("D:/study/opencvstudy/08/2.png");//读取参考图像
    
    Mat imageROI = image1(Rect(100,100,45,60));//兴趣区域
    cv::rectangle(image1, cv::Rect(100,100,45,60),cv::Scalar(0,255,255));
    ColorHistogram h;
    int minSat = 65;
    Mat colorhist = h.getHueHistogram(imageROI, minSat);//得到色调直方图
    ContentFinder finder;
    finder.setHistogram(colorhist);//把得到的直方图，用来实现直方图反向映射
    image1 = imread("D:/study/opencvstudy/08/2.png");//现在打开第二个图像用来定位ROI
    Mat hsv;
    cvtColor(image1, hsv, COLOR_BGR2HSV);
    int ch[1] = {0};
    finder.setThreshold(-1.0f);//不做阈值化
    Mat result = finder.find(hsv, 0.0f,
                             180.0f, ch);//查找
    Rect rect(100,100,45,60);
    cv::rectangle(image1, rect,cv::Scalar(0,0,255));

    TermCriteria criteria(TermCriteria::MAX_ITER,
                          10,0.01);
    cv::meanShift(result, rect, criteria);
    cv::rectangle(image1, rect,cv::Scalar(0,255,0));

    cv::namedWindow("result");
    cv::imshow("result",image1);
    cv::waitKey(0);

    return 0;
}
