import numpy as np
from numpy.linalg import inv


def covariance2d(sigma1, sigma2):
    cov1_2 = sigma1 * sigma2
    cov2_1 = sigma2 * sigma1
    cov_matrix = np.array([[sigma1 ** 2, cov1_2],
                           [cov2_1, sigma2 ** 2]])
    return np.diag(np.diag(cov_matrix))


x_observations = np.array([4000, 4260, 4550, 4860, 5110])
v_observations = np.array([280, 282, 285, 286, 290])

# Observation Errors
error_obs_x = 25  # Uncertainty in the measurement
error_obs_v = 6

# Process / Estimation Errors
error_est_x = 20
error_est_v = 5


z = np.c_[x_observations, v_observations]

n = len(z[0])

a = 2  # Acceleration
t = 1  # Difference in time


A = np.array([[1, t],
              [0, 1]])

B = np.array([[0.5 * t ** 2],
              [t]])

H = np.identity(n)

Q = np.zeros(n)

R = covariance2d(error_obs_x, error_obs_v)


# Initial State Matrix
X = np.array([[z[0][0]],
              [z[0][1]]])

# Initial Estimation Covariance Matrix
P = covariance2d(error_est_x, error_est_v)

for data in z[1:]:
    # prediction
    X = A.dot(X) + B.dot(a)

    # To simplify the problem, professor set off-diagonal terms to 0.
    P = np.diag(np.diag(A.dot(P).dot(A.T))) + Q

    # Calculating the Kalman Gain
    S = H.dot(P).dot(H.T) + R
    K = P.dot(H).dot(inv(S))

    # Reshape the new data into the measurement space.
    Y = H.dot(data).reshape(n, -1)

    # Update the State Matrix
    # Combination of the predicted state, measured values, covariance matrix and Kalman Gain
    X = X + K.dot(Y - H.dot(X))

    # Update Process Covariance Matrix
    P = (np.identity(len(K)) - K.dot(H)).dot(P)

print "Kalman Filter State Matrix:\n", X
