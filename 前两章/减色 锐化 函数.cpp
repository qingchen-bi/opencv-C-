/*
访问像素的多种方式
*/
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

/*void colorReduce(Mat image, int div = 64){//使用指针处理像素
    int nl = image.rows;
    int nc = image.cols * image.channels();//每行的元素数量
//    std::cout<<"this nc is "<<nc<<std::endl;
//    std::cout<<"this cols*elemsize() is "<<image.cols*image.elemSize()<<std::endl;
//    std::cout<<"this elemsize is "<<image.elemSize()<<std::endl;
    for(int j = 0;j<nl;j++){
        uchar* data = image.ptr<uchar>(j);

        for(int i=0;i<nc;i++){
         data[i] = data[i]/div*div + div/2;

        }
    }
}
*/
void colorReduce(Mat &image, int div = 64){//shi'yong迭代器遍历图像元素
    cv::Mat_<cv::Vec3b>::iterator it=
            image.begin<cv::Vec3b>();//获得在初始位置的迭代器
    cv::Mat_<cv::Vec3b>::iterator itend=
            image.end<Vec3b>();//获得结束位置
    for(; it!=itend; ++it){
        (*it)[0] = (*it)[0]/div * div + div/2;
        (*it)[1] = (*it)[1]/div * div + div/2;
        (*it)[2] = (*it)[2]/div * div + div/2;

    }
}

void sharpen(const cv::Mat &image, cv::Mat &result){
    result.create(image.size(), image.type());
    int nchannels = image.channels();
    for(int j=1;j<image.rows -1;j++){
        const uchar* previous=
                image.ptr<const uchar>(j-1);
        const uchar* current=
                image.ptr<const uchar>(j);
        const uchar* next=
                image.ptr<const uchar>(j+1);
        uchar* output = result.ptr<uchar>(j);
        for(int i = nchannels;i<(image.cols-1)*nchannels;i++){

            *output++ = cv::saturate_cast<uchar>(
                5*current[i]-current[i-nchannels]-
                current[i+nchannels]-previous[i]-next[i]);
    }
    }
    //把未处理的像素设为0
    result.row(0).setTo(cv::Scalar(0));
    result.row(result.rows-1).setTo(cv::Scalar(0));
    result.col(0).setTo(cv::Scalar(0));
    result.col(result.cols-1).setTo(cv::Scalar(0));


}

int main()
{
    cv::Mat image, result;
    std::cout<<"this image is "<<image.rows<<"x"<<image.cols<<std::endl;
    image = cv::imread("D:/study/opencvstudy/01/2.jpg", 0);
//    std::cout<<"this image is "<<image.rows<<"x"<<image.cols<<std::endl;
//    std::cout<<"this image is "<<image.step<<std::endl;//step = nc = cols *chanel.
//    cv::Mat result;
//    cv::flip(image, result, -9);//翻转
//    cv::namedWindow("Output Image");
//    cv::imshow("Output Image", result);

//    colorReduce(image, 64);
    sharpen(image, result);
    cv::namedWindow("Original Image");
    cv::imshow("Original Image", image);
    cv::namedWindow("Original result");
    cv::imshow("Original Image", result);
    cv::waitKey(0);
    return 0;
}
