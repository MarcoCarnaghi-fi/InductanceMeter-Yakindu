# -*- coding: utf-8 -*-
"""
Created on Thu Jan 17 17:00:03 2019

@author: pdaccs
"""

# Plot data from file

import matplotlib.pyplot as plt
import numpy as np

x, y = np.loadtxt('data1_several_cycles_coffe_10s_before_on.csv', delimiter=',', unpack=True, skiprows=1)
plt.plot(x,y, label='Temp coffee')

plt.xlabel('x')
plt.ylabel('y')
plt.title('Temp')
plt.legend()
plt.grid(b=True, which='major', axis='both')
plt.show()

x, y = np.loadtxt('data2_steam.csv', delimiter=',', unpack=True, skiprows=1)
plt.plot(x,y, label='Temp steam')

plt.xlabel('x')
plt.ylabel('y')
plt.title('Temp')
plt.legend()
plt.grid(b=True, which='major', axis='both')
plt.show()