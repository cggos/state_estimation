# State Estimation

* https://cggos.github.io/archive.html?tag=StateEstimation

-----

[TOC]

# 1. Probability Theory and Statistics

## MAP, MLE & MLS

* MAP (Maximum A Posteriori)

* MLE (Maximum Likelihood Estimate)
  - **模型已定，参数未知**：找出一组参数，使得模型产生出观测数据的概率最大，即找出一组能使似然函数最大的参数

* MLS (Minimum Least Squares)
  - OLS (Ordinary Least Squares)
  - WLS ([Weighted Least Squares](https://www.itl.nist.gov/div898/handbook/pmd/section4/pmd432.htm))

## RANSAC (RANdom SAmple Consensus)

## M-Estimator

* [M-estimators](http://www.statisticalconsultants.co.nz/blog/m-estimators.html)

* *MLESAC: A New Robust Estimator with Application to Estimating Image Geometry*

## Probabilistic Graphical Models

* [读懂概率图：你需要从基本概念和参数估计开始](https://www.jiqizhixin.com/articles/2017-11-29-3)

* *[Probabilistic Graphical Models (10-708, Spring 2014)](http://www.cs.cmu.edu/~epxing/Class/10708-14/lecture.html)*

* [Probabilistic inference and factor graphs](http://deepdive.stanford.edu/inference)

* [pgmpy](http://pgmpy.org/)

* [OpenGM](http://hciweb2.iwr.uni-heidelberg.de/opengm/): a C++ template library for discrete factor graph models and distributive operations on these models

### Factor Graph

### Bayesian Network

### Markov Random Fields


# 2. Filters


# 3. Optimization

## Basics

* Gauss Newton

* Levenberg-Marquadt
  - [sparseLM](http://users.ics.forth.gr/~lourakis/sparseLM/) : Sparse Levenberg-Marquardt nonlinear least squares in C/C++

* ESM

## Optimizer

* [immoptibox](http://www2.imm.dtu.dk/projects/immoptibox/): A MATLAB TOOLBOX FOR OPTIMIZATION AND DATA FITTING

* [NLopt](https://nlopt.readthedocs.io) is a library for nonlinear local and global optimization, for functions with and without gradient information

* [g2o](https://openslam-org.github.io/g2o.html): A General Framework for Graph Optimization
  - [Ubuntu 16.04.1 :: error while trying to run g2o_viewer](https://github.com/RainerKuemmerle/g2o/issues/133#issuecomment-265894146)
  - [g2o_viewer error](https://github.com/rainerkuemmerle/g2o/issues/151#issuecomment-281599307)
  - [SLAM Implementation: Bundle Adjustment with g2o](https://fzheng.me/2016/03/15/g2o-demo/)

* [Ceres Solver](http://ceres-solver.org/): an open source C++ library for modeling and solving large, complicated optimization problems
  - [Ceres-Solver 从入门到上手视觉SLAM位姿优化问题](https://blog.csdn.net/u011178262/article/details/88774577)

* [GTSAM](https://gtsam.org/)

* [TORO (Tree-based netwORk Optimizer)](https://openslam-org.github.io/toro.html) is an optimization approach for constraint-network

* [vertigo (Versatile Extensions for RobusT Inference using Graph Optimization)](https://github.com/christiankerl/vertigo/tree/master/trunk) is an extension library for g2o and gtsam 2.0

* [OR-Tools](https://developers.google.com/optimization/) is an open source software suite for optimization, tuned for tackling the world's toughest problems in vehicle routing, flows, integer and linear programming, and constraint programming.


## Bundle Adjustment

Sparse Hessian matrix :

<p align="center">
  <img src="images/mat_H.png"/>
</p>


# 4. Dataset

##  BAL Dataset

* Bundle Adjustment in the Large

##  g2o Dataset

* [g2o format datasets – Luca Carlone](https://lucacarlone.mit.edu/datasets/)
* sphere.g2o


# 5. Books

* ***[Probability, Statistics and Random Processes](https://www.probabilitycourse.com/)***

* ***STATE ESTIMATION FOR ROBOTICS***

* ***[Probabilistic Robotics](http://www.probabilistic-robotics.org/)***
  - [pptacher/probabilistic_robotics](https://github.com/pptacher/probabilistic_robotics): solution of exercises of the book "probabilistic robotics"

* ***Factor Graphs for Robot Perception***
