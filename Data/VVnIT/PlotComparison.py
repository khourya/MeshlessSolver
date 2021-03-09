import os
import PKUtilities as Util
import AnalyticalSolution as AS
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import axes3d

# Setup for analytical solutions
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

data_to_plot = 'unbound' # can be 'bound' or 'unbound'
solver = 'both' # runs either (1) 'cpp', (2) 'fortran', or (3) 'both' 
style = 'scatter' # 'contour' or 'scatter'

# Util.Execute(solver)

cpp_raw, cpp_sliced, t = Util.LoadCppData(data_to_plot)
fort_raw, fort_sliced, t = Util.LoadFortranData(data_to_plot)
R = cpp_sliced - fort_sliced

Util.PlotConcentrationComparison(cpp_sliced, fort_sliced, t, style)

# cpp_sliced, t = Util.TimeSlice(cpp_data)
# fort_data = np.loadtxt('..\LCMMOutput.txt', delimiter=',', skiprows=1)