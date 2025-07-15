"""
线性回归图  lmplot   或者 regplot
"""

# 导入库
import sys

import seaborn as sn
sn.set_style("whitegrid")

import matplotlib.pyplot as plt

names = sn.get_dataset_names()

print(f'dataset names: {names}')

#sys.exit(0)

# 导入自带数据
tips = sn.load_dataset("tips")  # csv 文件

# 单一分组参数
sn.lmplot(x="total_bill", y="tip", data=tips)
plt.savefig("./lmplot1.png", dpi=600)

# 分组的线性回归图，hue参数控制分类属性
sn.lmplot(x="total_bill", y="tip", hue="smoker", markers=["o", "*"],data=tips)
plt.savefig("./lmplot2.png", dpi=600)

# col+hue 双分组参数，既分组，又分子图绘制
# 同时控制axes 列数，及 size 尺寸
sn.lmplot(x="total_bill", y="tip", col="day", hue="day", data=tips, col_wrap=2, size=4)
plt.savefig("./lmplot3.png", dpi=600)

plt.show()