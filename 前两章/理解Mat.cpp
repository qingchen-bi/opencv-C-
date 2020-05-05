#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

Mat function(){
    Mat ima(500, 500, CV_8U, 50);
    return ima;
}

int main()
{
    namedWindow("Image 1");
    namedWindow("Image 2");
    namedWindow("Image 3");
    namedWindow("Image 4");
    namedWindow("Image 5");
    namedWindow("Imgae");

    Mat image1(240, 320, CV_8U, 100);

    imshow("Image", image1);
    waitKey(0);

    Mat image2(240, 320, CV_8UC3, Scalar(0, 0, 255));

    imshow("Image", image2);
    waitKey(0);

    Mat image3 = imread("D:/study/opencvstudy/01/1.jpg");
    Mat image4(image3);
    image1 = image3;//1  4 point to 3 Data

    image3.copyTo(image2);//闹明白这个以及和上下的区别。。。
    Mat image5 = image3.clone();//3's clone

    flip(image3, image3, 1);
    imshow("Image 3", image3);
    imshow("Image 1", image1);
    imshow("Image 2", image2);
    imshow("Image 4", image4);
    imshow("Image 5", image5);
    waitKey(0);
    Mat gray = function();
    imshow("Image", gray);
    image1 = imread("D:/study/opencvstudy/01/1.jpg", 0);
    image1.convertTo(image2, CV_32F, 1/255.0, 0.0);
    imshow("Image", image2);
    waitKey(0);

    return 0;
}
