/**
 * @file qr_test.cpp
 * @author cggos (cggos@outlook.com)
 * @brief QR Decomposition
 *        SuiteSparseQR实现更好一些，比自带Eigen::SparseQR快50%左右
 * @version 0.1
 * @date 2020-05-27
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <iostream>

#include <Eigen/Core>
#include <Eigen/SparseCore>
#include <Eigen/QR>
#include <Eigen/SparseQR>
#include <Eigen/SPQRSupport>
#include <Eigen/OrderingMethods>

using namespace Eigen;

int main() {

    // ================== SparseMatrix ==================

    // 初始化非零元素
    int r[3] = {0, 1, 2};
    int c[3] = {1, 2, 2};
    double val[3] = {6.1, 7.2, 8.3};
    std::vector<Triplet<double>> triplets;
    for (int i = 0; i < 3; ++i)
        triplets.emplace_back(r[i], c[i], val[i]);

    SparseMatrix<double, RowMajor> A0(4, 4);
    A0.setFromTriplets(triplets.begin(), triplets.end());
    std::cout << "A0 = \n" << A0 << std ::endl;

    // 初始化稀疏矩阵
    SparseMatrix<double> A(4, 4);
    A.setFromTriplets(triplets.begin(), triplets.end());
    std::cout << "A = \n" << A << std ::endl;


    // ================== SparseQR ==================

    // set ordering methods
    // AMDOrdering<double> ordering;
    // PermutationMatrix<Dynamic, Dynamic, double> perm;
    // ordering(A, perm); // call AMD

    SparseQR<SparseMatrix<double>, AMDOrdering<int>> qr;
    qr.compute(A);
    
    SparseMatrix<double> Q, R;
    // qr.matrixQ().evalTo(Q);
    Q = qr.matrixQ();
    R = qr.matrixR();

    std::cout << "Q: \n" << Q << std::endl;
    std::cout << "R: \n" << R << std::endl;


    

    // ================== SPQR ==================

    SPQR<SparseMatrix<double>> spqr;
    spqr.compute(A);
    
    // 求 Ax = b
    Vector4d b(1, 2, 3, 4);
    Vector4d x = spqr.solve(b);
    std::cout << "x  = " << x.transpose() << std::endl;
    std::cout << "Ax = " << (A*x).transpose() << std::endl;

    std::cout << std::endl;

    Matrix4d B1;
    B1 << 1, 2, 3, 0,
          8, 6, 0, 4,
          0, 0, 9, 7,
          5, 3, 0, 0;
    
    SparseMatrix<double> B = B1.sparseView();

    SPQR<SparseMatrix<double>> spqrhelper;
    spqrhelper.setSPQROrdering(SPQR_ORDERING_NATURAL);
    spqrhelper.compute(B);

    Matrix4d B2;
    // (spqrhelper.matrixQ().transpose() * B1).evalTo(B2);
    B2 = (spqrhelper.matrixQ().transpose() * B1);
    std::cout << "B2:\n" << B2 << std::endl;

    std::cout << std::endl;

    MatrixXd C1(9, 3);
    C1 << 1, 2, 3, 
          4, 5, 6, 
          7, 8, 9,
          0, 0, 0, 
          0, 0, 0, 
          3, 0, 4,
          0, 0, 0, 
          0, 0, 0, 
          0, 7, 8;

    Eigen::HouseholderQR<MatrixXd> hhqr_helper(C1);
    MatrixXd C_Q = hhqr_helper.householderQ();
    MatrixXd C_R = hhqr_helper.matrixQR();

    std::cout << "C_Q:\n" << C_Q << std::endl;
    std::cout << "C_R:\n" << C_R << std::endl;
    std::cout << "C_QtC:\n" << C_Q.transpose() * C1 << std::endl;

    return 0;
}
