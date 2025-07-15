#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Mon Jul 27 23:01:52 2020

@author: cg
"""

import numpy as np
import matplotlib.pyplot as plt

r = 2.0
a, b = (0., 0.)

#方法一：参数方程
theta = np.arange(0, 2*np.pi, 0.01)
x = a + r * np.cos(theta)
y = b + r * np.sin(theta)


fig = plt.figure() 
axes = fig.add_subplot(111) 
axes.plot(x, y)

axes.axis('equal')


# # 方法二：标准方程
# x = np.arange(a-r, a+r, 0.01)
# y = b + np.sqrt(r**2 - (x - a)**2)


# fig = plt.figure() 
# axes = fig.add_subplot(111) 
# axes.plot(x, y) # 上半部
# axes.plot(x, -y) # 下半部

# plt.axis('equal')
