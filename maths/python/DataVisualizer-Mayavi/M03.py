#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Sat Nov 12 08:44:17 2016

@author: gordon
"""

from numpy import *
from mayavi import mlab

x = [[-1,1,1,-1,-1],
[-1,1,1,-1,-1]]

y = [[-1,-1,-1,-1,-1],
[1,1,1,1, 1]]

z = [[1,1,-1,-1,1],
[1,1,-1,-1,1]]

s = mlab.mesh(x, y, z, representation="wireframe", line_width=1.0 )

mlab.show()