/*
 * @Author: Xiawenlong-bug 2473833028@qq.com
 * @Date: 2024-06-14 11:42:37
 * @LastEditors: Xiawenlong-bug 2473833028@qq.com
 * @LastEditTime: 2024-06-14 17:14:50
 * @FilePath: /VslamPractice/camera/undistortImages.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include<opencv2/opencv.hpp>
#include<string>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
    double k1 = -0.28340811, k2 = 0.07395907, p1 = 0.00019359, p2 = 1.76187114e-05;
    double fx = 458.654, fy = 457.296, cx = 367.215, cy = 248.375;
    Mat image = imread("../camera/distorted.png",CV_8UC1);
    if(image.data==nullptr){
        cerr << "CAN'T FIND IMAGE PATH" << endl;
        return -1;
    }
    int rows = image.rows, cols = image.cols;
    Mat image_distorted(rows,cols,CV_8UC1);
    for(int v=0; v<rows; v++){
        for (int u = 0; u < cols; u++){
            double x=(u-cx)/fx, y=(v-cy)/fy;
            double r=sqrt(x*x+y*y);
            double x_distorted = x * (1 + k1 * r * r + k2 * r * r * r * r) + 2 * p1 * x * y + p2 * (r * r + 2 * x * y);
            double y_distorted = y * (1 + k1 * r * r + k2 * r * r * r * r) + p1 * (r * r + 2 * x * y) + 2 * p2 * x * y;
            double u_distorted = fx * x_distorted + cx;
            double v_distorted = fy * y_distorted + cy;
            if(u_distorted>=0&&v_distorted>=0&&u_distorted<cols&&v_distorted<rows){
                image_distorted.at<uchar>(v,u)=image.at<uchar>((int)v_distorted,(int)u_distorted);
            }
            else{
                image_distorted.at<uchar>(v,u)=0;
            }
        }

    }
    //try opencv function
    Mat cv_distort_image(rows, cols, CV_8UC1);
    //Mat cameraMatrix(3,3,CV_8UC1);
    Mat cameraMatrix=(Mat_<double>(3,3)<<fx,0,cx,0,fy,cy,0,0,1);
    Mat distCoeffs = (Mat_<double>(1, 4) << k1, k2, p1, p2);
    undistort(image, cv_distort_image, cameraMatrix, distCoeffs);

    imshow("opencv_undistorted", cv_distort_image);
    imshow("undistorted", image);
    imshow("distorted", image_distorted);
    waitKey(0);
    return 0;


}