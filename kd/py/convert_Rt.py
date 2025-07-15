# -*- coding: utf-8 -*-
"""
convert R t
"""

import numpy as np
from numpy import transpose
from numpy.linalg import inv

from scipy.spatial.transform import Rotation as Rot

quat = [0.0017454, 0.0029307, -0.0019195, 0.9999923]

t = np.array([0.0203127935529, -0.00510325236246, -0.0112013882026]).reshape(-1,1)

Rm = Rot.from_quat(quat).as_matrix()
Ri = transpose(Rm).tolist()

mat_Ri = np.matrix(Ri)
mat_t = np.matrix(t)

Rt = np.dot(mat_Ri, -mat_t)

print(mat_Ri, mat_Ri.shape)
print(Rt)

