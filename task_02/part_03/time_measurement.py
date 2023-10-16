import subprocess
import time

# Ruta al programa en C que deseas ejecutar
programa_c = "./a.out"

params = {
    "a": 0,
    "b": 3.14159265359,
    "n": 100000000,
}

# Función para medir el tiempo de ejecución
def medir_tiempo_ejecucion():
    inicio = time.time()

    # Ejecuta el programa en C usando subprocess y redirige su salida estándar
    proceso = subprocess.Popen([programa_c], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    # Envía los parámetros al programa en C a través de la entrada estándar
    proceso.stdin.write(f"{params['a']}\n")
    proceso.stdin.write(f"{params['b']}\n")
    proceso.stdin.write(f"{params['n']}\n")
    proceso.stdin.close()

    # Espera a que el programa en C termine
    proceso.wait()

    fin = time.time()
    tiempo_ejecucion = fin - inicio

    return tiempo_ejecucion

# Llama a la función para medir el tiempo de ejecución
tiempo = medir_tiempo_ejecucion()

print(f"El programa en C se ejecutó en {tiempo} segundos.")
