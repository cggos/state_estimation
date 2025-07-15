import numpy as np
from numpy import transpose
from numpy.linalg import inv

from scipy.spatial.transform import Rotation as R

tbc = np.array([0, 0.4107, 0.1105])

Rbca = np.array([1, 0, 0, 0, 0, 1, 0, -1, 0]).reshape(3, 3)
Rcab = R.from_euler('zyx', [0, 0, -12], degrees=True).as_matrix()

Rbca = np.asmatrix(Rbca)

Rbc = np.dot(Rbca, Rcab)

print(Rbc)