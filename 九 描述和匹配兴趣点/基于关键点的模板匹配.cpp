#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
using namespace cv;
using namespace std;

int main()
{
    cv::Mat image1 = cv::imread("D:/study/opencvstudy/09/4.jpg");//读取参考图像
    cv::Mat image2 = cv::imread("D:/study/opencvstudy/09/4.jpg");//读取参考图像
    //定义关键点向量
    vector<KeyPoint> keypoint1;
    vector<KeyPoint> keypoint2;
    cv::Ptr<cv::FeatureDetector> fast = //定义特征点检测器
                cv::FastFeatureDetector::create (80, true,cv::FastFeatureDetector::TYPE_9_16 );
        fast -> detect(image1,keypoint1);//检测特征点
        fast -> detect(image2,keypoint2);

        const int nsize(11);//11*11的矩形，表示关键点周围的图像块
        Rect neighborhood(0, 0, nsize, nsize);
        Mat patch1;
        Mat patch2;
    //针对第一幅图像的每个关键点，在第二幅中找出最匹配的来
    Mat result;
    vector<DMatch> matches;//识别的匹配项储存在DMtch类型的向量中

    //图像一的全部关键点
    for(int i=0;i<keypoint1.size();i++){
        neighborhood.x = keypoint1[i].pt.x - nsize/2;
        neighborhood.y = keypoint1[i].pt.y - nsize/2;
    //如果超出图像的范围，就继续处理下一个点
        if(neighborhood.x<0 || neighborhood.y<0 ||
                neighborhood.x+nsize >= image1.cols ||
                neighborhood.y+nsize >= image1.rows
                )
            continue;
    //图像一的块
        patch1 = image1(neighborhood);
        DMatch bestMatch;//复位最匹配的值

        for(int j=0;j<keypoint2.size();j++){
            neighborhood.x = keypoint2[j].pt.x - nsize/2;
            neighborhood.y = keypoint2[j].pt.y - nsize/2;

            if(neighborhood.x<0 || neighborhood.y<0 ||
                    neighborhood.x+nsize >= image2.cols ||
                    neighborhood.y+nsize >= image2.rows
                    )
                continue;

            patch2 = image2(neighborhood);

            matchTemplate(patch1, patch2, result, 1);
            //检测 是否是最佳匹配
            if(result.at<float>(0,0) < bestMatch.distance){
                bestMatch.distance = result.at<float>(0,0);
                bestMatch.queryIdx = i;//int queryIdx –>是测试图像的特征点描述符（descriptor）的下标，同时也是描述符对应特征点（keypoint)的下标
                bestMatch.trainIdx = j;//int trainIdx –> 是样本图像的特征点描述符的下标，同样也是相应的特征点的下标。
            }
        }

    matches.push_back(bestMatch);
}
    nth_element(matches.begin(),matches.begin()+25,matches.end());
    //erase函数有问题？
   /* 为了使结果更具可读性， 在绘制匹配项时要限制线条的数量。 因此，
    我们只显示了差距最小的25个匹配项。 要想实现这个功能， 需要调
    用函数std::nth_element。 这个函数将排序后的第 N 个元素放在
    第 N 个位置， 比这个元素小的元素放在它的前面， 然后把向量中其
    余的元素清除。*/
    Mat matchImage;
    drawMatches(image1, keypoint1,
                image2,keypoint2,
                matches, matchImage,
                Scalar(255,255,255),
                Scalar(255,255,0));

    cv::namedWindow("matchresult",cv::WINDOW_NORMAL);
    cv::imshow("matchresult",matchImage);

    cv::waitKey(0);

    return 0;
}
