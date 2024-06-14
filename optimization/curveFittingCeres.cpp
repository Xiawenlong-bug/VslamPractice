/*
 * @Author: Xiawenlong-bug 2473833028@qq.com
 * @Date: 2024-06-14 20:17:56
 * @LastEditors: Xiawenlong-bug 2473833028@qq.com
 * @LastEditTime: 2024-06-14 22:56:44
 * @FilePath: /VslamPractice/optimization/curveFittingCeres.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include<iostream>
#include<vector>
#include<chrono>
#include<ceres/ceres.h>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>


using namespace std;

struct CURVE_FITTING_COST{
    CURVE_FITTING_COST(double x,double y):_x(x),_y(y){}

    template<typename T>
    bool operator()(
        const T *const abc,
        T *residual) const{
        residual[0] = T(_y) - ceres::exp(abc[0] * T(_x) * T(_x) + abc[1] * T(_x) + abc[2]);
        return true;
    }
    const double _x, _y;
};


int main(int argc, char **argv)
{
    double ar = 1.0, br = 2.0, cr = 1.0;
    double ae = 2.0, be = -1.0, ce = 5.0;
    int N = 100;

    double w_sigma = 1.0;
    double inv_sigma = 1.0/w_sigma;
    //random number generator
    cv::RNG rng;

    vector<double> x_data, y_data;
    for (int i = 0; i < N;i++)
    {
        double x = i / 100.0;
        x_data.push_back(x);
        y_data.push_back(exp(ar * x * x + br * x + cr) + rng.gaussian(w_sigma*w_sigma));
    }

    double abc[3] = {ae, be, ce};
    ceres::Problem problem;
    for(int i = 0; i < N; i++)
    {
        problem.AddResidualBlock(//添加误差项
    //第一个参数是误差计算函数，第二个参数是误差的维度，第三个参数是待估计参数的维度
            new ceres::AutoDiffCostFunction<CURVE_FITTING_COST, 1, 3>(
                new CURVE_FITTING_COST(x_data[i], y_data[i])),
            nullptr,//核函数，此处为空
            abc);//待估计参数
    }
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    ceres::Solve(options, &problem, &summary);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "solve time cost = " << time_used.count() << " seconds. " << endl;
    cout << summary.BriefReport() << endl;
    cout << "estimated a, b, c = " << abc[0] << ", " << abc[1] << ", " << abc[2] << endl;
    return 0;
    
}