#include <opencv2/core.hpp>
#include <math.h>

using namespace std;

class ROIExtracter
{
private:
    /* data */
    int *finger_roots;

public:
    ROIExtracter(int finger_roots[]);
    ~ROIExtracter();
    cv::Point **extract(int);
};

ROIExtracter::ROIExtracter(int finger_roots[])
{
    this->finger_roots = finger_roots;
}

ROIExtracter::~ROIExtracter()
{
}

cv::Point **ROIExtracter::extract(int roi_size)
{
    // int i = 1;
    // const int *const j = &i;
    // const int *const *p = &j;
    const int bias = 64;
    cv::Point **result = new cv::Point *[1];
    result[0] = new cv::Point[4];
    cv::Mat A = cv::Mat::zeros(cv::Size2d(2, 2), CV_32F);
    cv::Mat B = cv::Mat::zeros(cv::Size2d(4, 2), CV_32F);
    cv::Mat C = cv::Mat::zeros(cv::Size2d(4, 2), CV_32F);
    cv::Mat result_mat;
    int a = this->finger_roots[0];
    int b = this->finger_roots[1];
    int c = this->finger_roots[4];
    int d = this->finger_roots[5];
    float m = sqrt((a - c) * (a - c) + (b - d) * (b - d));

    A.at<float>(0, 0) = (a - c) / m;
    A.at<float>(0, 1) = (b - d) / m;
    A.at<float>(1, 0) = (d - b) / m;
    A.at<float>(1, 1) = (a - c) / m;

    B.at<float>(0, 0) = roi_size / 2.0;
    B.at<float>(0, 1) = roi_size / 2.0;
    B.at<float>(0, 2) = -roi_size / 2.0;
    B.at<float>(0, 3) = -roi_size / 2.0;
    B.at<float>(1, 0) = 0 + bias;
    B.at<float>(1, 1) = roi_size + bias;
    B.at<float>(1, 2) = roi_size + bias;
    B.at<float>(1, 3) = 0 + bias;

    C.at<float>(0, 0) = (a + c) / 2;
    C.at<float>(0, 1) = (a + c) / 2;
    C.at<float>(0, 2) = (a + c) / 2;
    C.at<float>(0, 3) = (a + c) / 2;
    C.at<float>(1, 0) = (b + d) / 2;
    C.at<float>(1, 1) = (b + d) / 2;
    C.at<float>(1, 2) = (b + d) / 2;
    C.at<float>(1, 3) = (b + d) / 2;

    //cv::gemm(A, B,1,C,1, result_mat);
    result_mat = A.inv() * B + C;

    result[0][0] = cv::Point(result_mat.at<float>(0, 0), result_mat.at<float>(1, 0));
    result[0][1] = cv::Point(result_mat.at<float>(0, 1), result_mat.at<float>(1, 1));
    result[0][2] = cv::Point(result_mat.at<float>(0, 2), result_mat.at<float>(1, 2));
    result[0][3] = cv::Point(result_mat.at<float>(0, 3), result_mat.at<float>(1, 3));

    // print(A);
    // printf("\n");
    // print(B);
    // printf("\n");
    // print(C);
    // printf("\n");
    // print(A.inv() * B);
    // printf("\n");
    // print(result_mat);
    // printf("\n");

    return result;
}