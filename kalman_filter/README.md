# Kalman Filter

* [The Kalman Filter](http://www.cs.unc.edu/~welch/kalman/)
* [Kalman Filter For Dummies](http://bilgin.esme.org/BitsAndBytes/KalmanFilterforDummies)
* [How a Kalman filter works, in pictures](http://www.bzarg.com/p/how-a-kalman-filter-works-in-pictures/)

<div align=center>
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/a/a5/Basic_concept_of_Kalman_filtering.svg/1920px-Basic_concept_of_Kalman_filtering.svg.png">
</div>

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

<div align=center>
  <img src="images/KalmanFilterMatrixProcessFlowchart.png">
</div>

## EKF

* [The Extended Kalman Filter: An Interactive Tutorial for Non-Experts](http://home.wlu.edu/~levys/kalman_tutorial/)


## Code

* [Understanding Kalman Filters with Python](https://medium.com/@jaems33/understanding-kalman-filters-with-python-2310e87b8f48)
* [FilterPy](https://filterpy.readthedocs.io/en/latest/) is a Python library that implements a number of Bayesian filters, most notably Kalman filters
* [pykalman](https://pykalman.github.io/): the dead-simple Kalman Filter, Kalman Smoother, and EM library for Python
* [rlabbe/Kalman-and-Bayesian-Filters-in-Python](https://github.com/rlabbe/Kalman-and-Bayesian-Filters-in-Python): Kalman Filter book using Jupyter Notebook

## Books

* *[卡尔曼滤波原理及应用：MATLAB仿真](http://yydz.phei.com.cn/book/eebook/%E5%8D%A1%E5%B0%94%E6%9B%BC%E6%BB%A4%E6%B3%A2%E5%8E%9F%E7%90%86%E5%8F%8A%E5%BA%94%E7%94%A8%EF%BC%9Amatlab%E4%BB%BF%E7%9C%9F/)*
