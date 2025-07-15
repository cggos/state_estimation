#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Sat Nov 12 09:06:35 2016

@author: gordon
"""

from numpy import mgrid, zeros, sin, pi, array
from mayavi import mlab

Lx = 4
Ly = 4

S = 10
dx = 0.05
dy = 0.05

# A mesh grid
X,Y = mgrid[-S:S:dx, -S:S:dy]

# The initial function:
#Z = zeros(X.shape)
#Z = sin(pi*X/Lx)*sin(pi*Y/Ly)
#Z[Z>0] = 1
#Z[Z<0] = -1

# The Fourier series:
W = zeros(X.shape)
m = 10
for i in xrange(1,m,2):
    for j in xrange(1,m,2):
        W += 1.0 / (i*j) * sin(i * pi * X / Lx) * sin(j * pi * Y / Ly)
W *= pi / 4.0


# prepare scene
scene = mlab.gcf()
# next two lines came at the very end of the design
scene.scene.magnification = 4

# plot the object
obj = mlab.mesh(X, Y, W)
P = mlab.pipeline

# first scalar_cut_plane
scalar_cut_plane = P.scalar_cut_plane(obj, plane_orientation='z_axes')
scalar_cut_plane.enable_contours = True
scalar_cut_plane.contour.filled_contours = True
scalar_cut_plane.implicit_plane.widget.origin = array([-0.025, -0.025,  0.48])
scalar_cut_plane.warp_scalar.filter.normal = array([ 0.,  0.,  1.])
scalar_cut_plane.implicit_plane.widget.normal = array([ 0.,  0.,  1.])
scalar_cut_plane.implicit_plane.widget.enabled = False # do not show the widget

# second scalar_cut-plane
scalar_cut_plane_2 = P.scalar_cut_plane(obj, plane_orientation='z_axes')
scalar_cut_plane_2.enable_contours = True
scalar_cut_plane_2.contour.filled_contours = True
scalar_cut_plane_2.implicit_plane.widget.origin = array([-0.025, -0.025,  -0.48])
scalar_cut_plane_2.warp_scalar.filter.normal = array([ 0.,  0.,  1.])
scalar_cut_plane_2.implicit_plane.widget.normal = array([ 0.,  0.,  1.])
scalar_cut_plane_2.implicit_plane.widget.enabled = False # do not show the widget

# see it from a closer view point
scene.scene.camera.position = [-31.339891336951567, 14.405281950904936, -27.156389988308661]
scene.scene.camera.focal_point = [-0.025000095367431641, -0.025000095367431641, 0.0]
scene.scene.camera.view_angle = 30.0
scene.scene.camera.view_up = [0.63072643330371414, -0.083217283169475589, -0.77153033000256477]
scene.scene.camera.clipping_range = [4.7116394000124906, 93.313165745624019]
scene.scene.camera.compute_view_plane_normal()

scene.scene.render()