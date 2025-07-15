#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Sat Nov 12 08:25:36 2016

@author: gordon

函数 z=abs(x)*exp(-x^2-(4y/3)^2) 的三维函数图象
"""

import numpy as np
from mayavi import mlab
x,y=np.ogrid [-2:2:160j,-2:2:160j]
z=abs(x)*np.exp(-x**2-(y/.75)**2)
pl=mlab.surf(x,y,z,warp_scale=2)
mlab.axes(xlabel='x',ylabel='y',zlabel='z')
mlab.outline(pl)
mlab.show()