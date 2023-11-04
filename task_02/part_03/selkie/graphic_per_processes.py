import matplotlib.pyplot as plt

# Lee los datos desde el archivo
x_values = []
y_values = []
with open("resultados_per_processes.txt", "r") as file:
    for line in file:
        parts = line.strip().split(": ")
        if len(parts) == 2:
            x_values.append(int(parts[0]))
            y_values.append(float(parts[1]))

# Crea una gráfica de líneas
plt.plot(x_values, y_values)
plt.xlabel('Número de procesos')
plt.ylabel('Tiempo (s)')
plt.title('Relación número de procesos - tiempo')

plt.savefig("resultados_per_processes.png")

plt.show()
