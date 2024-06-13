/*
 * @Author: Xiawenlong-bug 2473833028@qq.com
 * @Date: 2024-06-13 17:11:24
 * @LastEditors: Xiawenlong-bug 2473833028@qq.com
 * @LastEditTime: 2024-06-13 19:35:50
 * @FilePath: /VslamPractice/useEigen/eigenGeometry.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include<iostream>
#include<Eigen/Dense>
#include<Eigen/Core>
#include<Eigen/Geometry>
#include<cmath>

using namespace std;
using namespace Eigen;


int main(int argc, char** argv)
{
    Matrix3d rotation_matrix=Matrix3d::Identity();
    //沿Z轴旋转45度
    AngleAxisd rotation_vector(M_PI / 4, Vector3d(0, 0, 1));
    cout.precision(3);

    cout << "rotation_vector of 45 degree around (0,0,1) is : " << rotation_vector.matrix() << endl;
    //旋转向量与旋转矩阵的变换
    rotation_matrix=rotation_vector.toRotationMatrix();
    //rotation_vector.fromRotationMatrix(rotation_matrix);

    Vector3d v(1, 0, 0);
    Vector3d v_rotated = rotation_matrix * v;
    cout << "(1,0,0) after rotation is: " << v_rotated.transpose() << endl;

    //Euler angles
    Vector3d euler_angles = rotation_matrix.eulerAngles(2, 1, 0);//这里的(2,1,0)指的是ZYX顺序，即yaw pitch row
    cout << "yaw pitch row of the rotation matrix is: " << euler_angles.transpose() << endl;

    //欧式变换
    Isometry3d T=Isometry3d::Identity();
    T.rotate(rotation_vector);
    T.pretranslate(Vector3d(1, 3, 4));

    cout << "Transform matrix is: " << endl;
    cout << T.matrix() << endl;

    Vector3d v_transformed = T * v;
    cout << "(0,0,1) after transformation is: " << v_transformed.transpose() << endl;

    //Quaternion
    Quaterniond q = Quaterniond(rotation_vector);
    cout << "quaternion from rotation vector is: " << q.coeffs().transpose() << endl;
    //q.coeffs() 通常是指用于获取四元数（quaternion）中各个分量的方法。四元数通常由四个分量表示，通常按照 (x, y, z, w) 的顺序表示，其中 (x, y, z) 是虚部，w 是实部
    //eigen里用q来做旋转，在代码上表示如下

    v << 1, 0, 0;
    v_rotated = q * v;
    cout << "(1,0,0) after rotation is: " << v_rotated.transpose() << endl;

    
    return 0;

}