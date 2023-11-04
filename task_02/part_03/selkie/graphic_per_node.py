import matplotlib.pyplot as plt

# Lee los datos desde el archivo
data = {}
with open("resultados_per_node.txt", "r") as file:
    for line in file:
        parts = line.strip().split(": ")
        if len(parts) == 2:
            key, value = parts[0], float(parts[1])
            group = key.split("_")[0]

            if group not in data:
                data[group] = []
            
            data[group].append(value)

# Organiza los datos para la gráfica
groups = list(data.keys())
values = list(data.values())
labels = [f'{i+1} procesos por nodo' for i in range(len(values[0]))]

fig, ax = plt.subplots()
width = 0.2
x = range(len(groups))

for i in range(len(values[0])):
    ax.bar([pos + width * i for pos in x], [val[i] for val in values], width, label=labels[i])

ax.set_xlabel('Número de nodos')
ax.set_ylabel('Tiempo (s)')
ax.set_title('Resultados Selkie')
ax.set_xticks([pos + width for pos in x])
ax.set_xticklabels(groups)
ax.legend()

plt.savefig("resultados_per_node.png")

plt.show()
