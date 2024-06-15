/*
 * @Author: Xiawenlong-bug 2473833028@qq.com
 * @Date: 2024-06-15 09:51:33
 * @LastEditors: Xiawenlong-bug 2473833028@qq.com
 * @LastEditTime: 2024-06-15 20:06:35
 * @FilePath: /VslamPractice/optimization/pose_estimation_3d2d.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/calib3d/calib3d.hpp>
#include<Eigen/Core>
#include<ceres/ceres.h>
#include<ceres/rotation.h>
#include<sophus/se3.hpp>
#include<chrono>

using namespace std;
using namespace cv;

void find_feature_matches(const Mat &img_1, const Mat &img_2, vector<KeyPoint> &keypoints_1, vector<KeyPoint> &keypoints_2, vector<DMatch> &matches);

Point2d pixel2cam(const Point2d &p, const Mat &K);


Mat K = ( Mat_<double> ( 3,3 ) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1 );

struct SnavelyReprojectionError
{
    SnavelyReprojectionError(const Point2d uv,const Point3d xyz):_uv(uv),_xyz(xyz) {}

    template<typename T>
    bool operator()(const T* const camera, T* residual) const
    {
        T p[3];
        T point[3];

        point[0]=T(_xyz.x);
        point[1]=T(_xyz.y);
        point[2] = T(_xyz.z);
        ceres::AngleAxisRotatePoint(camera,point,p);
        
        p[0] += camera[3];
        p[1] += camera[4];
        p[2] += camera[5];

        T xp = p[0] / p[2];
        T yp = p[1] / p[2];

        T u_= xp*K.at<double>(0,0)+K.at<double>(0,2);
        T v_= yp*K.at<double>(1,1)+K.at<double>(1,2);
        
        residual[0] = T(_uv.x)-u_;
        residual[1] = T(_uv.y)-v_;
        return true;
    }
    static ceres::CostFunction* Create(const Point2d uv,const Point3d xyz) {
        return (new ceres::AutoDiffCostFunction<SnavelyReprojectionError, 2, 6>(
                new SnavelyReprojectionError(uv,xyz)));
    }

    

    Point2d _uv;
    Point3d _xyz;
};

int main(int argc, char** argv){
    if(argc!= 5){
        cerr << "fail to get enough arguments" << endl;
        return -1;
    }

    double camera[6] = {};

    Mat img_1 = imread(argv[1], IMREAD_COLOR);
    Mat img_2 = imread(argv[2],IMREAD_COLOR);
    assert(img_1.data&&img_2.data&&"cant load images");

    vector<KeyPoint> keypoints_1,keypoints_2;
    vector<DMatch> matches;
    find_feature_matches(img_1, img_2, keypoints_1, keypoints_2, matches);
    cout<<"keypoints matches has found : "<<matches.size()<<endl;

    Mat d1=imread(argv[3],IMREAD_UNCHANGED);
    Mat K = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);

    vector<Point3d> pts_3d;
    vector<Point2d> pts_2d;

    for(DMatch m:matches) {
        uchar d=d1.ptr<unsigned char>(int(keypoints_1[m.queryIdx].pt.y))[int (keypoints_1[m.queryIdx].pt.x)];
        if(d==0) continue;
        float dd = d / 5000.0;
        Point2d p1 = pixel2cam(keypoints_1[m.queryIdx].pt, K);
        pts_2d.push_back(keypoints_2[m.trainIdx].pt);
        pts_3d.push_back(Point3d(p1.x * dd, p1.y * dd, dd));
    }

    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    Mat r, t;
    solvePnP(pts_3d, pts_2d, K, Mat(), r, t, false,0);
    Mat R;
    cv::Rodrigues(r, R);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "solve pnp cost time: " << time_used.count() << " seconds." << endl;

    cout << "R = " << endl << R << endl;
    cout << "t = " << endl << t << endl;

    //Bundle adjustments by CERES
    ceres::Problem problem;
    for(int i = 0; i <pts_2d.size(); i++){
        ceres::CostFunction *cost_function = SnavelyReprojectionError::Create(pts_2d[i], pts_3d[i]);
        problem.AddResidualBlock(cost_function, NULL, camera);
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    cout << summary.BriefReport() << endl;

    Mat R_vec = (Mat_<double>(3, 1) << camera[0], camera[1], camera[2]);
    Mat R_cvest;
    Rodrigues(R_vec, R_cvest);
    cout << "R_cvest = " << endl << R_cvest << endl;

    return 0;
}

void find_feature_matches(const Mat &img_1, const Mat &img_2, vector<KeyPoint> &keypoints_1, vector<KeyPoint> &keypoints_2, vector<DMatch> &matches){
    Mat descriptors_1, descriptors_2;
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();

    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    detector->detect(img_1, keypoints_1);
    detector->detect(img_2, keypoints_2);

    descriptor->compute(img_1, keypoints_1, descriptors_1);
    descriptor->compute(img_2, keypoints_2, descriptors_2);

    vector<DMatch> match;
    matcher->match(descriptors_1, descriptors_2, match);

    double min_dis = 10000, max_dis = 0;
    for (int i = 0; i < descriptors_1.rows; i++) {
        double dist = match[i].distance;
        if (dist < min_dis) min_dis = dist;
        if (dist > max_dis) max_dis = dist;
    }

    cout << "max_dis = " << max_dis << endl;
    cout << "min_dis = " << min_dis << endl;

    for (int i = 0; i < descriptors_1.rows; i++) {
        if (match[i].distance <= max(2 * min_dis, 30.0)) {
            matches.push_back(match[i]);
        }
    }
    cout << matches.size() << endl;

}

Point2d pixel2cam(const Point2d &p, const Mat &K){
    return Point2d((p.x - K.at<double>(0, 2)) / K.at<double>(0, 0), (p.y - K.at<double>(1, 2)) / K.at<double>(1, 1));
}