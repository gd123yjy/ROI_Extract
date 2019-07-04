#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <fstream>
#include <iostream>

#include "ROIExtracter.h"

using namespace std;

vector<string> split(const string &str, const string &delim);

/*
input: fingers coordinates
output: ROI, represented by four points
*/
int main(int argc, char *argv[])
{
    const string img_load_base = argv[1]; //"/home/yjy/PycharmProjects/Finger_root_Eager/datasets/LHand/palmprint_trainval/";
    const string fg_load_path = argv[2];
    const string roi_save_path = argv[3];
    const string img_save_base = argv[4]; //"/home/yjy/dataset/palmprint_dectection/LHand/palmvein_ROI/";
    cv::Mat img;
    const int pt_num = 4;
    const int ROI_size = 212;
    char buff[200000];
    char out_buff[1000];
    int finger_roots[6] = {0};

    ifstream txt(fg_load_path);      //"/home/yjy/dataset/palmprint_dectection/LHand/txt_data/Figcon/figCon_tra.txt"
    ofstream txt_roi(roi_save_path); //"/home/yjy/dataset/palmprint_dectection/LHand/txt_data/ROI_with_bias/ROI_palmvein_xxFinger-xxx.txt"
    txt.read(buff, 200000);
    string figCon(buff);
    string outrow = "";
    vector<string>
        rows = split(figCon, "\n");

    for (int i = 0; i < rows.size(); i++)
    {
        string row = rows[i];
        vector<string> data = split(row, " ");
        string filename = data[0]; //.substr(0, 17);
        for (int j = 0; j < 6; j++)
        {
            finger_roots[j] = stoi(data[j + 1]);
        }
        ROIExtracter roie(finger_roots);

        try
        {
            img = cv::imread(img_load_base + filename);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            continue;
        }

        const cv::Point *const *result = roie.extract(ROI_size);
        cv::polylines(img, result, &pt_num, 1, true, cv::Scalar(0, 255, 0));
        cv::imwrite(img_save_base + filename, img);

        sprintf(out_buff, "%s %d %d %d %d %d %d %d %d\n", filename.c_str(), result[0][0].x, result[0][0].y, result[0][1].x, result[0][1].y, result[0][2].x, result[0][2].y, result[0][3].x, result[0][3].y);
        txt_roi << out_buff;

        delete[] result; // bad destruction
    }

    txt_roi.close();

    return 0;
}

vector<string> split(const string &str, const string &delim)
{
    vector<string> res;
    if ("" == str)
        return res;
    //先将要切割的字符串从string类型转换为char*类型
    char *strs = new char[str.length() + 1]; //不要忘了
    strcpy(strs, str.c_str());

    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while (p)
    {
        string s = p;     //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(NULL, d);
    }

    return res;
}