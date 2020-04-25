#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
using namespace cv;
using namespace std;

int main()
{
    cv::Mat image1 = cv::imread("D:/study/opencvstudy/09/scr.png");//读取参考图像
    cv::Mat target = cv::imread("D:/study/opencvstudy/09/tar2.jpg");//读取参考图像
     Mat result;
    // 定义搜索区域
    cv::Mat roi(image1,
    // 这里用图像的上半部分
    cv::Rect(0,0,image1.cols,image1.rows));
    cv::namedWindow("roi",cv::WINDOW_NORMAL);
    cv::imshow("roi",roi);

    // 进行模板匹配
    cv::matchTemplate(
    roi, // 搜索区域
    target, // 模板
    result, // 结果
    1); // 相似度
    // 找到最相似的位置
    double minVal, maxVal;
    cv::Point minPt, maxPt;
    cv::minMaxLoc(result, &minVal, &maxVal, &minPt, &maxPt);
    // 在相似度最高的位置绘制矩形
    // 本例中为minPt
    cv::rectangle(roi,
    cv::Rect(minPt.x, minPt.y, target.cols , target.rows),
    0);
    cv::namedWindow("matchresult",cv::WINDOW_NORMAL);
    cv::imshow("matchresult",roi);

    cv::waitKey(0);

    return 0;
}
