/*
8.1 8.2 8.3
Harris检测
考虑去除局部最大值附近距离的检测方法
Fast检测
*/
//头文件
#ifndef Detector_H
#define Detector_H
#include <opencv2/opencv.hpp>
class Harris{//harri类
private:
    cv::Mat cornerStrength;//opencv harris函数检测结果，每个像素的角点响应函数值
    cv::Mat cornerTH;//上一个的阈值化的结果
    cv::Mat localMax;//局部最大值结果
    int neighbourhood;//邻域窗口大小
    int aperture;//sobel边缘检测窗口大小（sobel获取各个像素点x，y方向的灰度导数
    double k;
    double maxStrength;//角点响应函数最大值
    double threshold;//阈值除去响应小的值
    int nonMaxSize;//默认3，就是最大值抑制的邻域窗口大小
    cv::Mat kernel;//最大值抑制的核，这里就是膨胀用的核
public:
    Harris():neighbourhood(3), aperture(3),
                     k(0.01), maxStrength(0.0),
                     threshold(0.01),nonMaxSize(3){

    }
    void setLocalMaxWindowsSize (int nonMaxSize){
        this ->nonMaxSize = nonMaxSize;
    }
    //检测Harris角点需要两个步骤。 首先是计算每个像素的Harris值
    //计算角点响应函数以及非最大值抑制
    void detect(const cv::Mat &image){
        cv::cornerHarris(image,cornerStrength,neighbourhood,aperture,k);//计算Harris
        double minStrength;
        //计算内部阈值
        cv::minMaxLoc(cornerStrength,&minStrength, &maxStrength);
        cv::Mat dilated;//临时图像
        //检测局部最大值
        //默认3*3膨胀，膨胀之后，除了局部最大值点和原来一样，其他非局部最大值点被
        //3*3邻域内的最大值点取代
        cv::dilate(cornerStrength,dilated,cv::Mat());
        //与原图相比，只剩下和原图值相同的点，这些点都是局部最大值点，保存到localMax
        cv::compare(cornerStrength,dilated,localMax,cv::CMP_EQ);
    }
    /*
     然后是用指定的阈值获得特征点。 因为Harris值的可选范围取决于选
    择的参数， 所以阈值被作为质量等级， 用最大Harris值的一个比例值
    表示：
     */
    //利用Harris值得到角点分布图
    cv::Mat getCornerMap(double qualityLevel){
        cv::Mat cornerMap;
        //对角点强度进行阈值化
        threshold = qualityLevel*maxStrength;
        cv::threshold(cornerStrength,cornerTH,threshold,255,cv::THRESH_BINARY);
        //转化为八位图像
        cornerTH.convertTo(cornerMap, CV_8U);
        //和局部最大值图与，剩下角点局部最大值图，即：完成非最大值抑制
        cv::bitwise_and(cornerMap, localMax, cornerMap);
        return cornerMap;
    }
    //检测角点
    void getCorner(std::vector<cv::Point> &points,
                   double qualityLevel){
        //获取角点图
        cv::Mat cornerMap = getCornerMap(qualityLevel);
        //获取角点
        getCorner(points, cornerMap);
    }
    //遍历全图，获取角点
    void getCorner(std::vector<cv::Point> &points,
                    const cv::Mat& cornerMap) {

            for( int y = 0; y < cornerMap.rows; y++ ) {
                    const uchar* rowPtr = cornerMap.ptr<uchar>(y);
                    for( int x = 0; x < cornerMap.cols; x++ ) {
                    // 非零点就是角点
                          if (rowPtr[x]) {
                                points.push_back(cv::Point(x,y));
                          }
                     }
                }
          }
    //标记角点
    void drawOnImage(cv::Mat &image,
                    const std::vector<cv::Point> &points,
            cv::Scalar color= cv::Scalar(255,255,255),
            int radius=3, int thickness=2) {
                    std::vector<cv::Point>::const_iterator it=points.begin();
                    while (it!=points.end()) {
                    // 角点处画圈
                    cv::circle(image,*it,radius,color,thickness);
                    ++it;
            }
    }
};

#endif // Detector_H
//main
#include <iostream>
#include <opencv2/opencv.hpp>
#include <Detector.h>
using namespace cv;
using namespace std;

int main()
{
    cv::Mat image1 = cv::imread("D:/study/opencvstudy/09/1.png",0);//读取参考图像
    Harris harris;
    harris.detect(image1);
    std::vector<cv::Point> pts;
    //不考虑抛除局部最大值的附近距离的角点检测
    //harris.getCorner(pts, 0.02);


    //不再用非极大值抑制了，而选取容忍距离，距离局部最大值的附近的角点去除
    //std::vector<cv::Point> corners;
    //cv::goodFeaturesToTrack(image1, // 输入图像
    //corners, // 角点图像
    //200, // 返回角点的最大数量
    //0.01, // 质量等级
    //10); // 角点之间允许的最短距离
    //harris.drawOnImage(image1, corners);

    //fast检测
    /*std::vector<cv::KeyPoint> keypoints;
    cv::Ptr<cv::FeatureDetector> fast = //定义特征点检测器
                cv::FastFeatureDetector::create (40, true,cv::FastFeatureDetector::TYPE_9_16 );
    fast -> detect(image1, keypoints);
    cv::drawKeypoints(image1, // 原始图像
    keypoints, // 关键点的向量
    image1, // 输出图像
    cv::Scalar(255,255,255), // 关键点的颜色
    cv::DrawMatchesFlags::DRAW_OVER_OUTIMG); // 画图标志
    */


    cv::namedWindow("matchresult",cv::WINDOW_NORMAL);
    cv::imshow("matchresult",image1);

    cv::waitKey(0);

    return 0;
}

