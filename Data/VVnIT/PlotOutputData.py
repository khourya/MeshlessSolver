import numpy as np
from matplotlib import pyplot as plt

data = np.loadtxt('LCMMOutput.csv', delimiter=',')
x = data[:, 1]
y = data[:, 2]

plt.subplot(1, 2, 1, aspect='equal')
plt.scatter(x, y, marker='.', s=7)
plt.xlabel('X [cm]')
plt.ylabel('Y [cm]')
length = len(x)
plt.title('N = {}'.format(length))
plt.tight_layout()
 
data = np.loadtxt('FortOut.csv', delimiter=',')
x = data[:, 3]
y = data[:, 4]

plt.subplot(1, 2, 2, aspect='equal')
plt.scatter(x, y, marker='.', s=7)
plt.xlabel('X [cm]')
plt.ylabel('Y [cm]')
length = len(x)
plt.title('N = {}'.format(length))
plt.tight_layout()


#plt.xlim([-0.05, 0.3])
#plt.ylim([-0.05, 0.3])

plt.show()