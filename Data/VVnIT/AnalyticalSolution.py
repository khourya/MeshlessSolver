import math
import random
import time
import csv
import os
import shutil
import subprocess
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import axes3d
from scipy.interpolate import griddata


def AnalyticalSolution(coeff, x, t):
    """
    :param coeff:
    :param x:
    :param t:
    :return:
    """
    D = coeff['D']
    Km = coeff['Km'] # * 1e-4
    Ke = coeff['Ke'] # * 1e-4
    K12 = coeff['K12'] # * 1e-4
    K21 = coeff['K21'] # * 1e-4
    Vt = coeff['Vt']  # vb = Vb/Vt
    Vb = coeff['Vb']  # vb = Vb/Vt
    vb = Vb/Vt

    K1 = -Km - K21
    K2 = K12 * vb
    K3 = -Ke - K12
    K4 = K21 / vb

    # Domain/Time Extents
    L = np.max(x)

    # Boundary Condition
    c_needle = 43

    # Number of Eigenfunctions to Evaluate
    N = 500

    # Steady Solution
    a = D
    b = 0
    cr = K1 - (K2*K4/K3)
    m = np.sqrt(-a*cr)/a

    eta = c_needle / (np.exp(m*L) - np.exp(-m*L))
    c_steady = eta * (np.exp(m*x) - np.exp(-m*x))

    # Transient
    c_transient = np.zeros_like(c_steady)

    for n in range(1, N + 1):
        ld = n*math.pi/L
        s1 = (K1 + K3 - D * ld**2) + math.sqrt((K1 - K3 - D * ld**2) ** 2 + 4 * K2 * K4)
        s2 = (K1 + K3 - D * ld**2) - math.sqrt((K1 - K3 - D * ld**2) ** 2 + 4 * K2 * K4)

        sig1 = (-1)**n * 2 * n * math.pi * s2 * c_needle / ((s2 - s1) * ((m**2 * L**2) + (n**2 * math.pi**2)))
        sig2 = -s1 * sig1 / s2

        c_transient = c_transient + ((sig1 * np.exp(s1 * t)) + sig2 * np.exp(s2 * t)) * np.sin(ld * x)

    c = c_steady + c_transient

    return c







