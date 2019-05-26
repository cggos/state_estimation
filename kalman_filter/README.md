# Kalman Filter

* [The Kalman Filter](http://www.cs.unc.edu/~welch/kalman/)
* [Kalman Filter For Dummies](http://bilgin.esme.org/BitsAndBytes/KalmanFilterforDummies)
* [Understanding Kalman Filters (mathworks)](https://www.mathworks.com/videos/series/understanding-kalman-filters.html)
* [THE KALMAN FILTER (ilectureonline)](http://www.ilectureonline.com/lectures/subject/SPECIAL%20TOPICS/26/190)
* [How a Kalman filter works, in pictures](http://www.bzarg.com/p/how-a-kalman-filter-works-in-pictures/)

-----

## Overview

**Kalman filter equations**

```
    /*------------------------------------------*\
     |  Kalman model                              |
     |                                            |
     |  state quation                             |
     |  x(k) = A.x(k-1)+B.u(k)+w(k-1)             |
     |                                            |
     |  observations equation                     |
     |  z(k) = H.x(k)+y(k)                        |
     |                                            |
     |  prediction equations                      |
     |  x^(k) = A.x^(k-1) + B.u(k)                |
     |  P^(k) = A.P(k-1).A^T + Q                  |
     |                                            |
     |  correction equations                      |
     |  K(k) = P^(k).H^T . (H.P^(k).H^T + R)^-1   |
     |  x(k) = x^(k) + K(k).(z(k) - H*x^(k))      |
     |  P(k) = (I - K(k).H).P^(k)                 |
     |                                            |
     \*------------------------------------------*/
```

卡尔曼滤波工作过程分为 **预测和测量更新**，根据上一时刻的状态和系统数学模型预测当前时刻的状态，然后将预测的状态与当前时刻的测量值进行“加权”，加权后的结果作为实际状态的最优估计。  

对于 **单传感器系统** 来说，预测部分用的是系统状态转移方程（数学模型），更新则使用传感器测量值。对于 **多传感器系统**，预测部分用的也是数学模型，更新则同步或异步更新各个传感器，比如接收到激光雷达测量值就更新距离状态，毫米波雷达测量值更新速度状态，这就达到了结合各个传感器的优势的目的。  

卡尔曼滤波是一种 **时域方法**，**适用于估计线性高斯动态系统的最优状态**，只需要知道前一时刻的状态和当前测量值来循环迭代得到当前时刻下状态的最优估计，计算量小，实时性好，因此在导航、目标定位和跟踪、信号处理甚至经济领域得到广泛的应用。对于非线性系统，可以用扩展卡尔曼滤波（Extended Kalman Filter）和无迹卡尔曼滤波（Unscented Kalman Filter）来估计系统状态。

* [卡尔曼滤波的直观理解](https://zhuanlan.zhihu.com/p/32253874)

## EKF

* [The Extended Kalman Filter: An Interactive Tutorial for Non-Experts](http://home.wlu.edu/~levys/kalman_tutorial/)


## Code

* [Understanding Kalman Filters with Python](https://medium.com/@jaems33/understanding-kalman-filters-with-python-2310e87b8f48)
* [FilterPy](https://filterpy.readthedocs.io/en/latest/) is a Python library that implements a number of Bayesian filters, most notably Kalman filters
* [pykalman](https://pykalman.github.io/): the dead-simple Kalman Filter, Kalman Smoother, and EM library for Python
* [rlabbe/Kalman-and-Bayesian-Filters-in-Python](https://github.com/rlabbe/Kalman-and-Bayesian-Filters-in-Python): Kalman Filter book using Jupyter Notebook

## Books

* *[卡尔曼滤波原理及应用：MATLAB仿真](http://yydz.phei.com.cn/book/eebook/%E5%8D%A1%E5%B0%94%E6%9B%BC%E6%BB%A4%E6%B3%A2%E5%8E%9F%E7%90%86%E5%8F%8A%E5%BA%94%E7%94%A8%EF%BC%9Amatlab%E4%BB%BF%E7%9C%9F/)*
