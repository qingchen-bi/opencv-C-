#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main()
{
    Mat image1 = imread("D:/study/opencvstudy/04/xk.jpg");

    Mat image2 = imread("D:/study/opencvstudy/04/8.jpg");
   // Mat image3 = imread("D:/study/opencvstudy/04/8.jpg");
    resize(image2, image2, Size(image1.cols, image1.rows));//重新定义尺寸

    std::vector<cv::Mat> planes;
    cv::split (image1, planes);//分割图像为三个单通道
    Mat result;
    //planes[0]+=image2;//将image2加到蓝色通道，但是有问题？
    cv::merge(planes, result);//逆向操作，三个单通道的图像合并成一个三通图像
   // Mat imageROI;
   // imageROI = image1(Rect(30, 20, image2.cols+0, image2.rows));
   // resize(image2, image2, Size(image1.cols, image1.rows));
   //addWeighted(image2,0.3,image1,0.9,0.,image1);//两个图像重合
    namedWindow("Image1");
    namedWindow("Image2");

    imshow("Image1", planes[0]);
    imshow("Image2", result);
    waitKey(0);
    return 0;
}
