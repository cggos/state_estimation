# Eigen

-----

## 基础

* 判断为空
  ```cpp
  MatrixXd A;
  A.isZero(0);
  ```

* cv::cv2eigen
    - 在 `#include <opencv2/core/eigen.hpp>` 之前必须先 `#include <Eigen/Core>`
    - 经笔者实验，该转换存在矩阵精度损失的问题，具体是由于什么原因造成的，待查更新

## 预定义宏

```cpp
EIGEN_STACK_ALLOCATION_LIMIT

EIGEN_MAX_ALIGN_BYTES
EIGEN_MAX_STATIC_ALIGN_BYTES

EIGEN_VECTORIZE_SSE2
EIGEN_VECTORIZE_NEON
```

## 内存分配

* Eigen矩阵默认 **列存储**


### 静态 内存分配

```cpp
Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor, 16, 1000> A400; // 静态分配内存
```

### 动态 内存分配

```cpp
Eigen::MatrixXd A;
```


## 字节对齐


## Vectorization

### 字节对齐

* Eigen::aligned_allocator
  ```cpp
  std::map<int, Eigen::Vector4f> 
  // -->
  std::map<int, Eigen::Vector4f, std::less<int>, Eigen::aligned_allocator<std::pair<const int, Eigen::Vector4f>>>
  ```

### SIMD

* SSE
* NEON 


## Lazy Evaluation and Aliasing

* https://eigen.tuxfamily.org/dox/TopicLazyEvaluation.html

```cpp
eval()

evalTo()

noalias()

XXXinPlace()
```

## 矩阵乘积

* 加速：分治法（矩阵分块乘积）
