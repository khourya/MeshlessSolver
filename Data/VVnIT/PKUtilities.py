import os
import subprocess
import numpy as np
import AnalyticalSolution as AS
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import axes3d
from scipy.interpolate import griddata

def TimeSlice(data):
    N_total, width = np.shape(data)
    t = [0]
    inds = [0]
    # headers = ['nTimeSteps', 'Time', 'nPoint', 'X', 'Y', 'C', 'Fx', 'Fy']

    # Finding locations to cut by time step
    for i in range(1, N_total):
        if data[i, 1] != data[i - 1, 1]:
            inds.append(i)  # indices of time change
            t.append(data[i, 1] / (60 ** 2))  # hours
            # print(t)

    # Slicing Data into separate time steps
    inds.append(N_total)
    N = int(N_total / len(t))  # number of samples in a time step
    SlicedData = np.zeros([N, width, len(t)])
    for i in range(len(t)):
        SlicedData[:, :, i] = data[inds[i]:inds[i + 1], :]

    return SlicedData, t


def LoadCppData(compartment):
    data_directory = 'C:\\Users\\Tony\\Documents\\GitRepos\\MeshlessSolver\\Data\\'

    if (compartment == 'bound'):
        data_directory += 'LCMMBoundSolution.txt'
    elif(compartment == 'unbound'):
        data_directory += 'LCMMUnboundSolution.txt'

    print(data_directory)
    rawData = np.loadtxt(data_directory, delimiter=',', skiprows=1)
    slicedData, t = TimeSlice(rawData)
    return rawData, slicedData, t

def LoadFortranData(compartment):
    data_directory = 'C:\\Users\\Tony\\Documents\\GitRepos\\MeshlessSolver\\Data\\'

    if (compartment == 'bound'):
        data_directory += 'VVnIT\\FOut\\LRCMM_PK_2D_Blood.csv'
    elif(compartment == 'unbound'):
        data_directory += 'VVnIT\\FOut\\LRCMM_PK_2D_Tissue.csv'

    print(data_directory)
    rawData = np.loadtxt(data_directory, delimiter=',', skiprows=0)
    slicedData, t = TimeSlice(rawData)
    return rawData, slicedData, t

def LoadData(data_type):
    _, cpp_sliced, t = LoadCppData(data_type)
    _, fort_sliced, t = LoadFortranData(data_type)
    x = cpp_sliced[:,3,0]
    y = cpp_sliced[:,4,0]

    return x, y, t, cpp_sliced, fort_sliced


def CreateAnalyticalSolution(data_sliced, parameter_set):
    pass

def GetParameters():
    input_directory = 'C:\\Users\\Tony\\Documents\\GitRepos\\MeshlessSolver\\Data\\LCMMInput.inp'
    with open(input_directory, 'r') as input_file:
        next_line_is_our_parameters = False
        while(not next_line_is_our_parameters):
            line = input_file.readline()
            if (line[0] == 'D'):
                next_line_is_our_parameters = True
        line = input_file.readline()

    # Remove line break at end
    line = line.split('\n')[0]

    # Split coefficients from string
    line = line.split('\t')
    N = len(line)
    values = np.zeros(N)
    for i in range(N):
        values[i] = float(line[i])

    coefficients = {'D': values[0],
          'Km':  values[1],
          'Ke':  values[2],
          'K12': values[3],
          'K21': values[4],
          'Vt':  values[5],  # vb = Vt/Vb
          'Vb':  values[6],  # vb = Vt/Vb
          }

    return coefficients


def PlotPK2D(x, y, c, t, fi, title):
    xm = np.unique(x)
    ym = np.unique(y)
    X, Y = np.meshgrid(xm, ym)
    c_contour = griddata((x, y), c, (X, Y), method='cubic')

    plt.figure(fi)
    plt.suptitle(title, fontsize=16)

    # Scatter
    plt.subplot(1, 2, 1)
    plt.scatter(x, y, c=c)
    plt.xlabel('x [cm]')
    plt.ylabel('y [cm]')
    plt.colorbar()
    # plt.clim(0, 43)
    plt.xlim([min(x), max(x)])
    plt.ylim([min(y), max(y)])
    plt.title('t = {} hrs'.format(t))
    # plt.tight_layout()

    # Contour
    plt.subplot(1, 2, 2)
    plt.contourf(X, Y, c_contour, 500, cmap='viridis', alpha=0.8)
    # plt.clim(0, 43)
    plt.xlabel('x [cm]')
    plt.ylabel('y [cm]')
    plt.colorbar()
    plt.xlim([min(x), max(x)])
    plt.ylim([min(y), max(y)])
    plt.title('t = {} hrs'.format(t))
    # plt.tight_layout()

def PlotSideBySide(x, y, c1, c2, t, fi, title, switch):
    xm = np.unique(x)
    ym = np.unique(y)
    X, Y = np.meshgrid(xm, ym)

    c1_contour = griddata((x, y), c1, (X, Y), method='cubic')
    c2_contour = griddata((x, y), c2, (X, Y), method='cubic')

    R = (c1 - c2)
    span = max(max(c1), max(c2)) - min(min(c1), min(c2))
    R = np.mean(R)/span
    title = title + ', R = {:2.4%}'.format(R*100)

    plt.figure(fi)
    plt.suptitle(title, fontsize=16)

    # Scatter
    if (switch == 'scatter'):
        plt.subplot(1, 2, 1)
        plt.scatter(x, y, c=c1)
        plt.xlabel('x [cm]')
        plt.ylabel('y [cm]')
        plt.colorbar()
        # plt.clim(0, 43)
        plt.xlim([min(x), max(x)])
        plt.ylim([min(y), max(y)])
        plt.title('t = {} hrs'.format(t))

        plt.subplot(1, 2, 2)
        plt.scatter(x, y, c=c2)
        plt.xlabel('x [cm]')
        plt.ylabel('y [cm]')
        plt.colorbar()
        # plt.clim(0, 43)
        plt.xlim([min(x), max(x)])
        plt.ylim([min(y), max(y)])
        plt.title('t = {} hrs'.format(t))
    
    elif (switch == 'contour'):
        plt.subplot(1, 2, 1)
        plt.contourf(X, Y, c1_contour, 500, cmap='viridis', alpha=0.8)
        plt.xlabel('x [cm]')
        plt.ylabel('y [cm]')
        plt.colorbar()
        # plt.clim(0, 43)
        plt.xlim([min(x), max(x)])
        plt.ylim([min(y), max(y)])
        plt.title('t = {} hrs'.format(t))

        plt.subplot(1, 2, 2)
        plt.contourf(X, Y, c2_contour, 500, cmap='viridis', alpha=0.8)
        plt.xlabel('x [cm]')
        plt.ylabel('y [cm]')
        plt.colorbar()
        # plt.clim(0, 43)
        plt.xlim([min(x), max(x)])
        plt.ylim([min(y), max(y)])
        plt.title('t = {} hrs'.format(t))
    
    # plt.tight_layout()

def PlotConcentrationComparison(cpp, fort, t, style):
    x = cpp[:,3,0]
    y = cpp[:,4,0]
    L = max(y)
    Params = GetParameters()

    for t_hr in t:
        t_hr = int(t_hr)
        t_sec = t_hr * 60 * 60
        index = int(t_hr)
        
        cpp_index = 3*index
        fort_index = 3*index + 1
        cpp_v_fort_index = 3*index + 2
        cpp_title = 'Numerical Solution - C++'
        fort_title = 'Numerical Solution - Fortran'
        cpp_v_fort_title = 'C++ versus Fortran'

        cpp_c = cpp[:, 5, t_hr]
        fort_c = fort[:, 5, t_hr]
        analytical_c = AS.AnalyticalSolution(Params, y, t_sec)
        R_cpp = np.abs(analytical_c - cpp_c)
        R_fort = np.abs(analytical_c - fort_c)

        print('Time Step: {} hrs'.format(t_hr))
        R = 100 * np.mean(R_cpp) / 43

        PlotSideBySide(x, y, cpp_c, analytical_c, t_hr, cpp_index, cpp_title, style)
        PlotSideBySide(x, y, fort_c, analytical_c, t_hr, fort_index, fort_title, style)
        PlotSideBySide(x, y, cpp_c, fort_c, t_hr, cpp_v_fort_index, cpp_v_fort_title, style)
        plt.show()

def Execute(solver):
    cpp_executable_directory = 'C:\\Users\\Tony\\Documents\\GitRepos\\MeshlessSolver\\MeshlessSolver.exe'
    fort_executable_directory = 'C:\\Users\\Tony\\Documents\\GitRepos\\MeshlessSolver\\Data\\VVnIT\\PKDiffusion.v2.exe'

    if (solver == 'cpp') or (solver == 'both'):
        print('Running C++ Meshless Solver...', end=' ')
        executable = subprocess.Popen(cpp_executable_directory)# , stdout=subprocess.DEVNULL)  #subprocess.FNULL) #, stdout=subprocess.PIPE, creationflags=0x08000000)
        executable.wait()
        print('done!')

    if (solver == 'fortran') or (solver == 'both'):
        print('Running Fortran Meshless Solver...', end=' ')
        executable = subprocess.Popen(fort_executable_directory)# , stdout=subprocess.DEVNULL)  #subprocess.FNULL) #, stdout=subprocess.PIPE, creationflags=0x08000000)
        executable.wait()
        print('done!')


def ExecutePK2D():
    executable = subprocess.Popen('PKDiffusion.v2.exe', stdout=subprocess.DEVNULL)  #subprocess.FNULL) #, stdout=subprocess.PIPE, creationflags=0x08000000)
    executable.wait()