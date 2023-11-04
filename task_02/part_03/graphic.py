import re
import matplotlib.pyplot as plt
import os

program_names = []
average_times = []

with open('output.txt', 'r') as file:
    for line in file:
        match = re.search(r'\[Average time (.*?)\] (\d+\.\d+) seconds', line)
        if match:
            program_name = match.group(1)
            average_time = float(match.group(2))
            program_names.append(program_name)
            average_times.append(average_time)

plt.bar(program_names, average_times)
plt.xlabel('Implementaciones')
plt.ylabel('Tiempo medio (segundos)')
plt.title('Tiempo medio en ejecutar las implementaciones')
plt.xticks(rotation=45, ha='right')
plt.tight_layout()

output_file_path = 'grafica_barras.png'

if os.path.exists(output_file_path):
    os.remove(output_file_path)

plt.savefig(output_file_path)

plt.show()