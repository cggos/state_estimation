#include <eigen3/unsupported/Eigen/CXX11/Tensor>
#include <iostream>

int main() {
  Eigen::Tensor<float, 3> t_3d(2, 3, 4);

  std::cout << t_3d << std::endl;

  std::cout << t_3d.NumDimensions << std::endl;

  // Resize t_3d by assigning a tensor of different sizes, but same rank.
  // t_3d = Eigen::Tensor<float, 3>(3, 4, 3);

  // Map a tensor of ints on top of stack-allocated storage.
  int storage[128] = {6};  // 2 x 4 x 2 x 8 = 128
  Eigen::TensorMap<Eigen::Tensor<int, 4>> t_4d(storage, 2, 4, 2, 8);
  // The same storage can be viewed as a different tensor. You can also pass the sizes as an array.
  Eigen::TensorMap<Eigen::Tensor<int, 2>> t_2d(storage, 16, 8);
  // You can also map fixed-size tensors.  Here we get a 1d view of the 2d fixed-size tensor.
  Eigen::TensorFixedSize<float, Eigen::Sizes<4, 3>> t_4x3;
  Eigen::TensorMap<Eigen::Tensor<float, 1>> t_12(t_4x3.data(), 12);

  std::cout << t_4d << std::endl;

  // Initialize all elements to random values.
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 3; ++j) {
      for (int k = 0; k < 4; ++k) {
        t_3d(i, j, k) = 3;
      }
    }
  }
  // Set the value of the element at position (0, 1, 0);
  t_3d(0, 1, 0) = 12.0f;

  std::cout << t_3d << std::endl;

  Eigen::Tensor<float, 2> a(30, 40);
  Eigen::Tensor<float, 2> b(30, 40);
  Eigen::Tensor<float, 2> c = a + b;
  // Eigen::Tensor<float, 2> c(30, 50);

  Eigen::DefaultDevice dev0;
  // Eigen::ThreadPoolDevice dev1(4);
  // Eigen::GpuDevice dev2;

  c.device(dev0) = a + b;
  // c.device(dev0) = a.contract(b, Eigen::array<Eigen::IndexPair<Eigen::Index>, 1>{{{1, 0}}});  // ?

  std::cout << c.NumDimensions << std::endl;

  // 如果if矩阵中的对应元素为1，那么返回的矩阵的对应元素选择then中对应的元素值，否则选择else中的元素值
  Eigen::Tensor<bool, 3> if1;
  Eigen::Tensor<float, 3> then1;
  Eigen::Tensor<float, 3> else1;
  Eigen::Tensor<float, 3> result = if1.select(then1, else1);

  return 0;
}
