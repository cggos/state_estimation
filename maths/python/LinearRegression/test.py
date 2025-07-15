#!/usr/bin/env python
# coding=utf-8

# sudo pip install -U scikit-learn pandas
# sudo pip install xlrd # for xls
# sudo pip install openpyxl

import numpy as np
import pandas as pd
# import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression 

def abline(slope, intercept):
    """Plot a line from slope and intercept"""
    axes = plt.gca()
    x_vals = np.array(axes.get_xlim())
    y_vals = intercept + slope * x_vals
    plt.plot(x_vals, y_vals, '--')

# x1 = [[2.8512], [2.9138], [2.7057], [2.0984], [2.678], [2.133]]
# y1 = [[9], [6], [9], [12], [11], [11]]

data_xls = pd.read_excel('./test.xls') # index_col='ID', sheet_name='Sheet1'
data_RL = pd.DataFrame(data_xls, columns= ['Rectus_L'])
data_Time = pd.DataFrame(data_xls, columns= [u'延迟回路'])

# print data_RL # data_xls['Rectus_L']

val_RL = data_RL.values
val_Tm = data_Time.values

val_RL_01 = val_RL[:6]
val_Tm_01 = val_Tm[:6]
val_RL_02 = val_RL[-12:]
val_Tm_02 = val_Tm[-12:]

# slope, intercept = np.polyfit(x1, y1, 1)

linreg = LinearRegression()

linreg.fit(val_RL_01, val_Tm_01)
slop01 = linreg.coef_[0][0]
intercept01 = linreg.intercept_[0]

linreg.fit(val_RL_02, val_Tm_02)
slop02 = linreg.coef_[0][0]
intercept02 = linreg.intercept_[0]

# print linreg.intercept_[0]
# print linreg.coef_[0][0]
# print linreg.score(val_RL_01, val_Tm_01)

plt.scatter(val_RL_01, val_Tm_01, marker = '^', color = 'b', label='Group01', s = 30)
plt.scatter(val_RL_02, val_Tm_02, marker = 'o', color = 'r', label='Group02', s = 30)

abline(slop01, intercept01)
abline(slop02, intercept02)

plt.xlabel("Rectus_L")
plt.ylabel("CB time")
plt.title('Rectus_L - CB time')
plt.legend(loc = 'upper right')

# sns.regplot(x=val_RL_01, y=val_Tm_01, color="g")

plt.show()

