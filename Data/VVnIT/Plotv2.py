import os
import PKUtilities as Util
import AnalyticalSolution as AS
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import axes3d

# Setup
                 #      AwM,  AnM,  EwM,   EnM,   KwM,  KnM
Coefficients = {'Km':  [5.61, 0.00, 5.61,  0.00,  5.61, 0.00],
                'Ke':  [1.58, 1.58, 0.79,  0.79,  1.50, 1.50],
                'K12': [2.19, 2.19, 2.12,  2.12,  6.22, 6.22],
                'K21': [1.11, 1.11, 2.78,  2.78,  3.94, 3.94],
                'Vt':  [5.18, 5.18, 13.14, 13.14, 8.34, 8.34],
                'Vb':  [2.63, 2.63, 27.39, 27.39, 6.47, 6.47]
                }

Params = {'D': 7.8e-8,
          'Km':  0.,
          'Ke':  0.,
          'K12': 0.,
          'K21': 0.,
          'Vb':  0.,  # vb = Vt/Vb
          'Vt':  0.,  # vb = Vt/Vb
          'rho': 0.
          }

nCases = len(Coefficients)

cpp_data = np.loadtxt('LCMMOutput.txt', delimiter=',', skiprows=1)
# fortran_data = np.loadtxt('LRCMM_PK_2D_Tissue.csv', delimiter=',')
cpp_sliced, t = Util.TimeSlice(cpp_data)
# fortran_sliced, t = Util.TimeSlice(fortran_data)

Params['Km'] = Coefficients['Km'][4]
Params['Ke'] = Coefficients['Ke'][4]
Params['K12'] = Coefficients['K12'][4]
Params['K21'] = Coefficients['K21'][4]
Params['Vb'] = Coefficients['Vb'][4]
Params['Vt'] = Coefficients['Vt'][4]

for t_hr in range(9):
    index = t_hr
    x = cpp_sliced[:, 3, t_hr]
    y = cpp_sliced[:, 4, t_hr]

    L = max(y)
    t_sec = t_hr * 60 * 60
    cpp_title = 'Numerical Solution - C++'
    fortran_title = 'Numerical Solution - Fortran'

    cpp_c = cpp_sliced[:, 5, t_hr]
    analytical_c = AS.AnalyticalSolution(Params, y, t_sec)
    R_cpp = np.abs(analytical_c - cpp_c)

    print('Time Step: {} hrs'.format(t_hr))
    R = 100 * np.mean(R_cpp) / 43



    # Util.PlotPK2D(x, y, cpp_c, t_hr, 2 * t_hr, cpp_title)
    # Util.PlotPK2D(x, y, fortran_c, t_hr, 2 * t_hr + 1, fortran_title)
    Util.PlotPK2D(x, y, cpp_c, t_hr, 2*index, 'Fortran vs. C++')


    # c_analytical = AS.AnalyticalSolution(Params, x, t_sec)
    # plt.scatter(L - x, c_analytical, label='AS at {} hours'.format(t_hr))
    # plt.scatter(L - x, cpp_c, label='C++ at {} hours'.format(t_hr))
    # plt.legend()
    # plt.draw()

plt.show()
