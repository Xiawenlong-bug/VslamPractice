/*
 * @Author: Xiawenlong-bug 2473833028@qq.com
 * @Date: 2024-06-15 22:58:33
 * @LastEditors: Xiawenlong-bug 2473833028@qq.com
 * @LastEditTime: 2024-06-16 10:38:43
 * @FilePath: /VslamPractice/bundle_adjustment/bundle_adjustment_ceres.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <ceres/ceres.h>
#include "common.h"
#include "SnavelyReprojectionError.h"


using namespace std;

void SolveBA(BALProblem &problem);

int main(int argc, char** argv) {
    if (argc != 2)
    {
        cout << "usage: bundle_adjustment_ceres bal_data.txt" << endl;
        return 1;
    }

    BALProblem bal_problem(argv[1]);
    bal_problem.Normalize();
    bal_problem.Perturb(0.1, 0.5, 0.5);
    bal_problem.WriteToPLYFile("initial.ply");
    SolveBA(bal_problem);
    bal_problem.WriteToPLYFile("final.ply");

    return 0;   
}

void SolveBA(BALProblem &bal_problem) {
    const int point_block_size =bal_problem.point_block_size();
    const int camera_block_size =bal_problem.camera_block_size();
    double *points =bal_problem.mutable_points();
    double *cameras =bal_problem.mutable_cameras();

    const double *observations =bal_problem.observations();
    ceres::Problem problem;
    
    for (int i = 0; i < bal_problem.num_observations(); ++i) {
        ceres::CostFunction *cost_function;
        cost_function = SnavelyReprojectionError::Create(observations[i * 2], observations[i * 2 + 1]);

        ceres::LossFunction *loss_function= new ceres::HuberLoss(1.0);

        double *camera = cameras + camera_block_size * bal_problem.camera_index()[i];
        //camera_block_size = bal_problem.camera_block_size();
        //*camera = cameras + bal_problem.camera_block_size() * bal_problem.camera_index()[i]
        double *point = points + point_block_size * bal_problem.point_index()[i];
        //point_block_size = bal_problem.point_block_size();
        //*point = points + bal_problem.point_block_size() * bal_problem.point_index()[i]

        problem.AddResidualBlock(cost_function, loss_function, camera, point);//这里只能使用对象的方式，使用指针是不行的。

    }

    cout << "bal problem loaded successfully" << endl;
    std::cout << "bal problem have " << bal_problem.num_cameras() << " cameras and "
              << bal_problem.num_points() << " points. " << std::endl;//bal_problem.num_cameras()表示相机位姿个数
              //bal_problem.num_points()表示路标点数
    std::cout << "Forming " << bal_problem.num_observations() << " observations. " << std::endl;// bal_problem.num_observations()表示观测点数
 
    std::cout << "Solving ceres BA ... " << endl;//BA求解

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_SCHUR;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.FullReport() << endl;

    
}

// void SolveBA(BALProblem &bal_problem) {
//     const int point_block_size = bal_problem.point_block_size();
//     const int camera_block_size = bal_problem.camera_block_size();
//     double *points = bal_problem.mutable_points();
//     double *cameras = bal_problem.mutable_cameras();
 
//     // Observations is 2 * num_observations long array observations 
//     // [u_1, u_2, ... u_n], where each u_i is two dimensional, the x
//     // and y position of the observation.
//     const double *observations = bal_problem.observations();
//     ceres::Problem problem;
 
//     for (int i = 0; i < bal_problem.num_observations(); ++i) 
//     {
//         ceres::CostFunction *cost_function;
 
//         // Each Residual block takes a point and a camera as input
//         // and outputs a 2 dimensional Residual
//         //形参为-----observed_x，observed_y
//         cost_function = SnavelyReprojectionError::Create(observations[2 * i + 0], observations[2 * i + 1]);//调用SnavelyReprojectionError.h c文件
 
//         // If enabled use Huber's loss function.
//         ceres::LossFunction *loss_function = new ceres::HuberLoss(1.0);//核函数
 
//         // Each observation corresponds to a pair of a camera and a point
//         // which are identified by camera_index()[i] and point_index()[i]
//         // respectively.
//         double *camera = cameras + camera_block_size * bal_problem.camera_index()[i];
//         //camera_block_size = bal_problem.camera_block_size();
//         //*camera = cameras + bal_problem.camera_block_size() * bal_problem.camera_index()[i]
//         double *point = points + point_block_size * bal_problem.point_index()[i];
//         //point_block_size = bal_problem.point_block_size();
//         //*point = points + bal_problem.point_block_size() * bal_problem.point_index()[i]
//         problem.AddResidualBlock(cost_function, loss_function, camera, point); // 向问题中添加误差项
//         //CostFunction* : 描述最小二乘的基本形式即代价函数
//         //LossFunction* : 描述核函数的形式
//     }
 
//     // show some information here ...
//     std::cout << "bal problem file loaded..." << std::endl;//输出bal problem file loaded...
//     std::cout << "bal problem have " << bal_problem.num_cameras() << " cameras and "
//               << bal_problem.num_points() << " points. " << std::endl;//bal_problem.num_cameras()表示相机位姿个数
//               //bal_problem.num_points()表示路标点数
//     std::cout << "Forming " << bal_problem.num_observations() << " observations. " << std::endl;// bal_problem.num_observations()表示观测点数
 
//     std::cout << "Solving ceres BA ... " << endl;//BA求解
//     ceres::Solver::Options options; // 这里有很多配置项可以填Options类嵌入在Solver类中 ，在Options类中可以设置关于求解器的参数
//     options.linear_solver_type = ceres::LinearSolverType::SPARSE_SCHUR; 
//     // 增量方程如何求解 这里的linear_solver_type 是一个Linear_solver_type的枚举类型的变量
//     //使用Schur消元
//     options.minimizer_progress_to_stdout = true;
//     ceres::Solver::Summary summary;// 优化信息
//     ceres::Solve(options, &problem, &summary);
//     std::cout << summary.FullReport() << "\n";
// }