import matplotlib.pyplot as plt
import numpy as np

x = np.arange(2, 11, 1)
y = np.array([[2.245, 1.055], [2.227, 1.053], [1.690, 1.046], [1.686, 0.729],
              [1.680, 0.534], [1.620, 0.446], [1.636, 0.394], [1.654, 0.355],
              [1.475, 0.314]])

plt.plot(x, y)
plt.xlabel('x')
plt.ylabel('y')
plt.title('Relación numero de procesadores y tiempo de ejecución')
plt.show()

