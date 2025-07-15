#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from scipy import fft

Fs = 200                         # sampling rate
Ts = 1.0/Fs                      # sampling interval
t = np.arange(0, 1, Ts)            # time vector
ff = 5                          # frequency of the signal

zero = np.zeros(10)
zeros = np.zeros(Fs/ff/2)
ones = np.ones(Fs/ff/2)
count = 0
y = []
for i in range(Fs):
    if i % Fs/ff/2 == 0:
        if count % 2 == 0:
            y = np.append(y,zeros)
        else:
            y = np.append(y,ones)
        count += 1

plt.subplot(2, 1, 1)
plt.plot(t, y, 'k-')
plt.xlabel('time')
plt.ylabel('amplitude')

plt.subplot(2, 1, 2)
n = len(y)                       # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T  # two sides frequency range
freq = frq[range(n/2)]           # one side frequency range

Y = np.fft.fft(y)/n              # fft computing and normalization
Y = Y[range(n/2)]

plt.plot(freq, abs(Y), 'r-')

plt.show()
