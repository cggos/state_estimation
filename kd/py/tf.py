import numpy as np
from numpy import transpose
from numpy.linalg import inv

from scipy.spatial.transform import Rotation as R

T_imu_c0 = np.array([-1, 0, 0, 0.05143,
                     0, 1, 0, -0.00453,
                     0, 0, -1, -0.01503,
                     0, 0, 0, 1]).reshape(4, 4)

T_body_imu = np.array([-1, 0, 0, 0.05143,
                       0, -0.139173, -0.990268, 0.419486,
                       0, -0.990268, 0.139173, 0.247606,
                       0, 0, 0, 1]).reshape(4, 4)

T_imu_c0 - np.asmatrix(T_imu_c0)
T_body_imu = np.asmatrix(T_body_imu)

T_body_c0 = np.dot(T_body_imu, T_imu_c0)

# T = np.matrix('-1, 0, 0, 0.05143; 0, -0.139173, -0.990268, 0.419486; 0, -0.990268, 0.139173, 0.247606; 0, 0, 0, 1')

print(T_body_c0)
