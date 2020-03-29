/*
把颜色检测算法封装到类中
Author：qingchen—bi
Date：2020.3.29
*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class ColorDetector
{
public:
    //空构造函数
    ColorDetector() : maxDist(100), target(0, 0, 0) {}

    Mat process(const Mat &image){
        result.create(image.size(), CV_8U);//必要时重新分配二值映像
        //取得迭代器
        Mat_<Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
        Mat_<Vec3b>::const_iterator itend = image.end<Vec3b>();
        Mat_<uchar>::iterator itout = result.begin<uchar>();

        for( ; it!=itend; ++it, ++itout){//比较与目标颜色的差距
            if(getDistanceToTargetColor(*it)<=maxDist){
              *itout = 255;
            }else {
               *itout = 0;
            }
        }
        return result;
    }
    //计算与目标颜色的距离
    int getDistanceToTargetColor(const Vec3b& color) const {
        return getColorDistance(color, target);
    }
    //计算两个颜色的城区距离
    int getColorDistance(const Vec3b& color1, const Vec3b& color2) const {
        return abs(color1[0] - color2[0] +
                    abs(color1[1] - color2[1] +
                    abs(color1[2] - color2[2])));
    }
    //设置颜色差距的阈值
    void setColorDistanceThreshold(int distance){
        if(distance < 0)
            distance = 0;
        maxDist = distance;
    }
    //得到颜色差距的阈值
    int getColorDistanceThreshold() const {
        return maxDist;
    }
    //设置目标颜色
    void setTargetColor(uchar blue, uchar green, uchar red){
        target = Vec3b(blue, green, red);
    }
    //第二种方法
    void setTargetColor(cv::Vec3b color){
        target = color;
    }
    //得到需要检测的颜色
    Vec3b getTargetColor() const {
        return target;
    }
private:
    int maxDist;//允许的最小差距
    Vec3b target;//目标颜色
    Mat result;//存储二值映像结果的图像
};

int main()
{
    ColorDetector cdetect;//创建图像处理器对象

    Mat image1 = imread("D:/study/opencvstudy/04/8.jpg");
    if(image1.empty()){
        return 0;
    }
    cdetect.setTargetColor(230, 190, 130);//设置输入参数

    namedWindow("result");
    imshow("result", cdetect.process(image1));
    waitKey(0);

    return 0;
}
