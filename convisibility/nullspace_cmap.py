from matplotlib import cm
import matplotlib.pyplot as plt
import numpy as np

dx, dy = 0.05, -0.05
x = np.arange(0.0, 2.0, dx)
y = np.arange(2.0, 0.0, dy)

X, Y = np.meshgrid(x, y)
extent = np.min(x), np.max(x), np.min(y), np.max(y)


def Z(x, y):
    return (x * y - 1) ** 2


def Dx(x, y):
    return 2 * y * (x * y - 1)


def Dy(x, y):
    return 2 * x * (x * y - 1)


def Zl(x, y, x0=0.0, y0=0.0):
    return Z(x0, y0) + Dx(x0, y0) * (x - x0) + Dy(x0, y0) * (y - y0)


def E0(x, y):
    return Z(x, y) + Z(x, y)


def E1(x, y):
    return Zl(x, y, 0.5, 1.4) + Zl(x, y, 1.2, 0.5)


Zs = [E0(X, Y), Zl(X, Y, 0.5, 1.4), Zl(X, Y, 1.2, 0.5), E1(X, Y)]

# ax=plt.pcolor(X,Y,E0(X, Y),cmap='jet', vmin=0,vmax=18)
# cbar=plt.colorbar()

fig, axes = plt.subplots(1, 4)
fig.suptitle('nullspace', fontsize=20)
for ax, Z in zip(axes.flat, Zs):
    im = ax.imshow(Z, cmap=cm.jet, alpha=1.0, interpolation='bilinear', extent=extent)
    fig.colorbar(im, ax=ax, shrink=0.4)

# cax = fig.add_axes([0.15, 0.15, 0.7, 0.05])
# fig.colorbar(im, cax=cax, shrink=0.95, orientation="horizontal")  # ax=axes.ravel().tolist() location='bottom'

# ax = plt.gca()  # 获取到当前坐标轴信息
# ax.xaxis.set_ticks_position('top')  # 将X坐标轴移到上面
# ax.invert_yaxis()  # 反转Y坐标轴

# plt.colorbar()
# plt.style.use('classic')
# cmap = plt.cm.get_cmap("jet")
# plt.set_cmap(cm.jet)

plt.show()
