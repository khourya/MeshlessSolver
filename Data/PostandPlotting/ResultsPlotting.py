import os

import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import axes3d

import AnalyticalSolution as AS
import PKUtilities as Util

print(os.getcwd())

# compartment == 'bound' or 'unbound'
_, boundData, t = Util.LoadCppData('bound')
_, unboundData, t = Util.LoadCppData('unbound')

print(np.shape(boundData))
print(np.shape(unboundData))
