#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void salt(cv::Mat image, int n){//椒盐噪声
    int i, j;
    for(int k = 0;k<n;k++){
        i = std::rand()%image.cols;
        j = std::rand()%image.rows;
                if(image.type() == CV_8UC1){
            image.at<uchar>(j,i) = 255;
        }else if(image.type() == CV_8UC3){
            image.at<Vec3b>(j,i)[0] = 255;
            image.at<Vec3b>(j,i)[1]= 255;
            image.at<Vec3b>(j,i)[2] = 255;//模板类的知识。。。
        }
    }
}
int main()
{
    Mat image = imread("D:/study/opencvstudy/01/1.jpg");
    salt(image, 1000);

    namedWindow("Image");
    imshow("Image", image);
    waitKey(0);
    return 0;
}
