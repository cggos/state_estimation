#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
@reference: Implementation of Kalman Filter with Python Language
"""

from numpy import *
from numpy import dot, sum, tile
from numpy.linalg import inv, det
from numpy.random import randn
import pylab as pl


def kf_predict(X, P, A, Q, B, U):
    X = dot(A, X) + dot(B, U)
    P = dot(A, dot(P, A.T)) + Q
    return (X, P)


def kf_update(X, P, Y, H, R):
    IM = dot(H, X)
    IS = R + dot(H, dot(P, H.T))
    K = dot(P, dot(H.T, inv(IS)))
    X = X + dot(K, (Y-IM))
    P = P - dot(K, dot(IS, K.T))
    LH = gauss_pdf(Y, IM, IS)
    return (X, P, K, IM, IS, LH)


def gauss_pdf(X, M, S):
    if M.shape[1] == 1:
        DX = X - tile(M, X.shape[1])
        E = 0.5 * sum(DX * (dot(inv(S), DX)), axis=0)
        E = E + 0.5 * M.shape[0] * log(2 * pi) + 0.5 * log(det(S))
        P = exp(-E)
    elif X.shape[1] == 1:
        DX = tile(X, M.shape[1])- M
        E = 0.5 * sum(DX * (dot(inv(S), DX)), axis=0)
        E = E + 0.5 * M.shape[0] * log(2 * pi) + 0.5 * log(det(S))
        P = exp(-E)
    else:
        DX = X-M
        E = 0.5 * dot(DX.T, dot(inv(S), DX))
        E = E + 0.5 * M.shape[0] * log(2 * pi) + 0.5 * log(det(S))
        P = exp(-E)
    return (P[0], E[0])


# time step of mobile movement
dt = 0.1

# Initialization of state matrices
X = array([[0.0], [0.0], [0.1], [0.1]])
P = diag((0.01, 0.01, 0.01, 0.01))
A = array([[1, 0, dt, 0], [0, 1, 0, dt], [0, 0, 1, 0], [0, 0, 0, 1]])

Q = eye(X.shape[0])
B = eye(X.shape[0])
U = zeros((X.shape[0], 1))

# Measurement matrices
Y = array([[X[0, 0] + abs(randn(1)[0])], [X[1, 0] + abs(randn(1)[0])]])
H = array([[1, 0, 0, 0], [0, 1, 0, 0]])
R = eye(Y.shape[0])

x1 = [X[0]]
x2 = [X[1]]
y1 = [Y[0]]
y2 = [Y[1]]

# Number of iterations in Kalman Filter
N_iter = 50

# Applying the Kalman Filter
for i in arange(0, N_iter):
    (X, P) = kf_predict(X, P, A, Q, B, U)
    (X, P, K, IM, IS, LH) = kf_update(X, P, Y, H, R)
    Y = array([[X[0, 0] + abs(0.1 * randn(1)[0])], [X[1, 0] + abs(0.1 * randn(1)[0])]])
    x1.append(X[0])
    x2.append(X[1])
    y1.append(Y[0])
    y2.append(Y[1])

pl.plot(x1, x2)
pl.plot(y1, y2)
pl.show()
