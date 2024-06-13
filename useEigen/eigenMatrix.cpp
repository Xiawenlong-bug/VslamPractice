#include<iostream>
#include<Eigen/Dense>
#include<Eigen/Core>

using namespace std;
using namespace Eigen;

#define MATRIX_SIZE 60
int main(int argc, char *argv[]) {
    Matrix<float, 2, 3> matrix_23;
    Vector3d v_3d;
    Matrix3d matrix_33 = Matrix3d::Zero();
    Matrix<double,Dynamic,Dynamic> matrix_dynamic;
    MatrixXd matrix_x;

    matrix_23 << 1, 2, 3, 4, 5, 6;
    cout << matrix_23 << endl;
    //横行，竖列，填入时优先行
    for(int i = 0; i < matrix_23.rows(); i++) {
        for(int j = 0; j < matrix_23.cols(); j++){
            cout<< matrix_23(i, j);
        }
        cout << endl;
    }

    v_3d << 3, 2, 1;
    Matrix<double, 3, 1> vd_3d;
    vd_3d << 4, 5, 6;

    Matrix<double, 2, 1> result = matrix_23.cast<double>() * v_3d;//eigen需要显式地进行格式转换
    cout<<result << endl;

    matrix_33 = Matrix3d::Random();
    cout << "random matrix 3*3 is " << matrix_33 << endl;

    //矩阵求取特征值
    SelfAdjointEigenSolver<Matrix3d> eigen_solver(matrix_33.transpose() * matrix_33);//求某个矩阵的特征值&特征向量，就用SelfAdjointEigenSolver
    cout << "Eigen values = " << eigen_solver.eigenvalues() << endl;
    cout << "Eigen vectors = " << eigen_solver.eigenvectors() << endl;

    //优化：Cholesky分解&QR分解
    //Cholesky分解要求正定矩阵
    //此处略过

}
