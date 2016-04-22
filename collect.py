''' Collect and plot data from lab4 and 5 '''

import subprocess
import csv
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
from mpl_toolkits.mplot3d import Axes3D
from scipy.interpolate import griddata

X = []
Y = []
Z = []
for i in range(10):
    for w in range(9):
        X.append(2**w)
        Y.append(2**i)
        p1 = subprocess.Popen(['./lab_4/lab4', '-w', str(2**w), '-j', '100'],
                              stdout=subprocess.PIPE)
        p2 = subprocess.check_output(['./lab_5/lab5', '-m', '-s', str(2**i)],
                                     stdin=p1.stdout)
        p2 = p2.decode('ASCII')
        reader = csv.reader(p2.split('\n'), delimiter=',')
        for row in reader:
            print(row[3])
            Z.append(float(row[3]))
            break
print(X, Y, Z)


xyz = {'x': X, 'y': Y, 'z': Z}

df = pd.DataFrame(xyz, index=range(len(xyz['x'])))

x1 = np.linspace(df['x'].min(), df['x'].max(), len(df['x'].unique()))
y1 = np.linspace(df['y'].min(), df['y'].max(), len(df['y'].unique()))
x2, y2 = np.meshgrid(x1, y1)
z2 = griddata((df['x'], df['y']), df['z'], (x2, y2), method='cubic')

fig = plt.figure()
ax = fig.gca(projection='3d')
ax.view_init(elev=10, azim=130)
surf = ax.plot_surface(x2, y2, z2, rstride=1, cstride=1, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)
ax.set_zlim(0, 100)

ax.zaxis.set_major_locator(LinearLocator(11))
ax.zaxis.set_major_formatter(FormatStrFormatter('%d'))
ax.set_xlabel('Length of phase')
ax.set_ylabel('Page table entries')
ax.set_zlabel('Page fault %')

fig.colorbar(surf, shrink=0.5, aspect=5)
plt.title('Relationship Between Dankness and Upvotes')

plt.savefig('out.png')
