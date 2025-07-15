"""
通过 梯度下降法 计算 rosenbrock函数最小值，理论上最小在 f(1,1)=0。

Rosenbrock函数：是一个用来测试最优化算法性能的非凸函数，由Howard Harry Rosenbrock在1960年提出[1]。也称为Rosenbrock山谷或Rosenbrock香蕉函数，也简称为香蕉函数
"""

import numpy as np

def cal_rosenbrock(x1, x2):
    """
    计算rosenbrock函数的值
    :param x1:
    :param x2:
    :return:
    """
    return (1 - x1) ** 2 + 100 * (x2 - x1 ** 2) ** 2


def cal_rosenbrock_prax(x1, x2):
    """
    对x1求偏导
    """
    return -2 + 2 * x1 - 400 * (x2 - x1 ** 2) * x1

def cal_rosenbrock_pray(x1, x2):
    """
    对x2求偏导
    """
    return 200 * (x2 - x1 ** 2)

def for_rosenbrock_func(max_iter_count=100000, step_size=0.001):
    pre_x = np.zeros((2,), dtype=np.float32)
    loss = 10
    iter_count = 0
    while loss > 0.001 and iter_count < max_iter_count:
        error = np.zeros((2,), dtype=np.float32)
        error[0] = cal_rosenbrock_prax(pre_x[0], pre_x[1])
        error[1] = cal_rosenbrock_pray(pre_x[0], pre_x[1])

        for j in range(2):
            pre_x[j] -= step_size * error[j]

        loss = cal_rosenbrock(pre_x[0], pre_x[1])  # 最小值为0

        print("iter_count: ", iter_count, "the loss:", loss)
        iter_count += 1
    return pre_x

if __name__ == '__main__':
    w = for_rosenbrock_func()
    print(w)