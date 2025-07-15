# -*- coding: utf-8 -*-
"""
Created on Wed Nov  9 08:14:03 2016

@author: Gordon
"""

from numpy import *;

data1=mat(zeros((3,3)));
#创建一个3*3的零矩阵，矩阵这里zeros函数的参数是一个tuple类型(3,3)
data2=mat(ones((2,4)));
#创建一个2*4的1矩阵，默认是浮点型的数据，如果需要时int类型，可以使用dtype=int
data3=mat(random.rand(2,2));
#这里的random模块使用的是numpy中的random模块，random.rand(2,2)创建的是一个二维数组，需要将其转换成#matrix
data4=mat(random.randint(10,size=(3,3)));
#生成一个3*3的0-10之间的随机整数矩阵，如果需要指定下界则可以多加一个参数
data5=mat(random.randint(2,8,size=(2,5)));
#产生一个2-8之间的随机整数矩阵
data6=mat(eye(2,2,dtype=int));
#产生一个2*2的对角矩阵
data7=mat(diag([1,2,3]));
#生成一个对角线为1、2、3的对角矩阵

data8=mat(eye(2,2)*0.5);
data9=data8.I;
#求矩阵matrix([[0.5,0],[0,0.5]])的逆矩阵