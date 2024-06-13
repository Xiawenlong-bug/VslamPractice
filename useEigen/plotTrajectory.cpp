/*
 * @Author: Xiawenlong-bug 2473833028@qq.com
 * @Date: 2024-06-13 19:37:58
 * @LastEditors: Xiawenlong-bug 2473833028@qq.com
 * @LastEditTime: 2024-06-13 21:17:53
 * @FilePath: /VslamPractice/useEigen/plotTrajectory.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include<iostream>
#include<Eigen/Dense>
#include<Eigen/Core>
#include<Eigen/Geometry>
#include<string>
#include<vector>
#include<unistd.h>
#include<fstream>


using namespace std;
using namespace Eigen;

string trajectory_file = "./trajectory.txt";
void DrawTrajectory(vector<Isometry3d, aligned_allocator<Isometry3d>>);


int main(int argc, char **argv)
{
    /*让我详细解释一下：

    std::vector<Isometry3d>：这是一个标准模板库（STL）中的动态数组，用于存储 Isometry3d 类型的对象。但是，这只是一个基本声明，默认情况下，std::vector 使用其默认的分配器来管理内存。
    Eigen::aligned_allocator<Isometry3d>：这是 Eigen 库提供的一个特殊的分配器，用于确保存储在内存中的对象（在这种情况下是 Isometry3d）是对齐的。
    在许多现代硬件和库（如 Eigen）中，数据的对齐对于性能至关重要。特别地，对于 SIMD（单指令多数据）操作，数据对齐通常是必需的。
    Isometry3d 很可能是 Eigen 库中用于表示3D空间中的等距变换（即旋转和平移）的类型。由于这些变换通常包含矩阵或向量，因此确保这些矩阵或向量在内存中正确对齐是非常重要的。
    综上所述，这行代码的目的是创建一个 std::vector，该向量使用 Eigen::aligned_allocator 来确保存储的 Isometry3d 对象在内存中正确对齐，从而可能提高后续使用这些对象的性能。*/
    vector<Isometry3d, Eigen::aligned_allocator<Isometry3d>> poses;

    ifstream fin(trajectory_file);

    if (!fin){
        cout<<"Error! failed to open trajectory file"<<endl;
        return -1;
    }

    while (!fin.eof())
    {
        double time, tx, ty, tz, qx, qy, qz, qw;
        fin >> time >> tx >> ty >> tz >> qx >> qy >> qz >> qw;

        Isometry3d Twr = Isometry3d(Quaterniond(qx, qy, qz, qw));
        Twr.pretranslate(Vector3d(tx, ty, tz));
        poses.push_back(Twr);
    }
    cout << "total number of the poses in trajectory " << poses.size() << endl;
    DrawTrajectory(poses);
    return 0;
}


void DrawTrajectory(vector<Isometry3d, aligned_allocator<Isometry3d>> poses)
{
    //略，涉及Pangolin的使用
}