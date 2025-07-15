#include <Eigen/Jacobi>
#include <iostream>

int main() {
  Eigen::Matrix<double, 5, 5> mat = Eigen::Matrix<double, 5, 5>::Random();

  std::cout << "mat:\n" << mat << std::endl;

  Eigen::JacobiRotation<double> temp_GR;
  for (int n = 0; n < mat.cols(); ++n) {
    for (int m = (int)mat.rows() - 1; m > n; m--) {
      // Givens matrix G
      temp_GR.makeGivens(mat(m - 1, n), mat(m, n));
      // Multiply G to the corresponding lines (m-1,m) in each matrix
      // Note: we only apply G to the nonzero cols [n:mat.cols()-n-1], while
      //       it is equivalent to applying G to the entire cols [0:mat.cols()-1].
      (mat.block(m - 1, n, 2, mat.cols() - n)).applyOnTheLeft(0, 1, temp_GR.adjoint());
    }
  }

  std::cout << "mat (Applied GR):\n" << mat << std::endl;

  return 0;
}