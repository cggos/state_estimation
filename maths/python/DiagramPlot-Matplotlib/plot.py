#!/usr/bin/env python
# --*-- coding:utf-8 --*--

import numpy as np
import matplotlib.pyplot as plt 
 
x=range(-1000,0)
y=[1.0/e for e in x]
plt.plot(x,y)

x=range(1,1001)
y=[1.0/e for e in x]
plt.plot(x,y)

plt.show()


pai =np.pi
x = np.arange(0,2*pai,0.01)
y = np.sin(x)
plt.style.use('ggplot')
plt.plot(x,y)
plt.show()

