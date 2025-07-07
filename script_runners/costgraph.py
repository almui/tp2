import os
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path


# Path to CSVs
directory = "script_runners"

# Get all CSV files
csv_files = [f for f in os.listdir(directory) if f.endswith(".csv")]

results = {}
def clientes(nombre_instancia):
    num_str = ""

    for char in nombre_instancia:
        if char.isdigit():
            num_str += char
        else:
            break  # detenemos cuando ya no hay dígitos


    return int(num_str)


for filename in csv_files:
    filepath = os.path.join(directory, filename)
    df = pd.read_csv(filepath)

    if "capacity_ok" not in df.columns:
        print(f"Skipping {filename}, 'capacity_ok' column not found.")
        continue

    # Normalize 'capacity_ok' to boolean
    df['capacity_ok'] = df['capacity_ok'].astype(str).str.strip().str.lower() == "true"
    instancias ={}
    for index, row in df.iterrows():
        if row['capacity_ok']==False or row['instance']=="E048-04y.dat": #no inlcuyo un valor muy alto 
            print(f"Skipping {row['instance']}, not valid.")
            continue
        num_items = clientes(row['instance'][1:])
        instancias[num_items]=row['total_cost']


    results[filename.replace(".csv", "")] = instancias


# Separate and order results
insertion_results = {k.replace("insertion", "insertion"): v for k, v in results.items() if k.startswith("insertion")}
cw_results = {k.replace("clarke_wright", "cw"): v for k, v in results.items() if k.startswith("clarke_wright")}





# Colores lindos y distintos para cada línea
colores = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd", "#8c564b", "#17becf"]

# Acceder a los resultados
cw_1 = cw_results['cw']  # clarke_wright
cw_2 = cw_results['cw+relocate']
cw_3 = cw_results['cw+relocate+swap']
cw_4 = cw_results['cw+swap']

in_1 = insertion_results['insertion']
in_2 = insertion_results['insertion+relocate']
in_3 = insertion_results['insertion+swap']

# Ordenar los datos por número de ítems (clave)
x1, y1 = zip(*sorted(cw_1.items()))
x2, y2 = zip(*sorted(cw_2.items()))
x3, y3 = zip(*sorted(cw_3.items()))
x4, y4 = zip(*sorted(cw_4.items()))
x5, y5 = zip(*sorted(in_1.items()))
x6, y6 = zip(*sorted(in_2.items()))
x7, y7 = zip(*sorted(in_3.items()))

# Graficar
plt.figure(figsize=(12, 7))

plt.plot(x1, y1, label='cw', color=colores[0], marker='o', linestyle='--')
plt.plot(x2, y2, label='cw+relocate', color=colores[1], marker='s', linestyle='--')
plt.plot(x3, y3, label='cw+relocate+swap', color=colores[2], marker='^', linestyle='--')
plt.plot(x4, y4, label='cw+swap', color=colores[3], marker='x', linestyle='--')
plt.plot(x5, y5, label='insertion', color=colores[4], marker='D', linestyle='--')
plt.plot(x6, y6, label='insertion+relocate', color=colores[5], marker='P', linestyle='--')
plt.plot(x7, y7, label='insertion+swap', color=colores[6], marker='*', linestyle='--')

plt.xlabel("Número de ítems")
plt.ylabel("Costo")
plt.title("Costos por instancia - 7 algoritmos")
plt.legend()
plt.grid(True, which="both", ls="--", linewidth=0.5)
plt.tight_layout()

output_path = os.path.join(directory, "costos_7_algoritmos.png")
plt.savefig(output_path)
plt.show()

i=0
# Graficar cada uno por separado 
df = pd.DataFrame([
    {"Algoritmo": algoritmo, "NumItems": num_items, "Costo": costo}
    for algoritmo, costos in results.items()
    for num_items, costo in costos.items()
])
plt.figure(figsize=(12, 7))
for algoritmo, grupo in df.groupby("Algoritmo"):
    grupo = grupo.sort_values("NumItems")
    
    plt.figure(figsize=(10, 6))  # Crea una nueva figura
    plt.plot(grupo["NumItems"], grupo["Costo"], marker='o', linestyle='--', label=algoritmo, color=colores[i])
    i+=1
    plt.xlabel("Número de ítems")
    plt.ylabel("Costo ")
    plt.title(f"Costos por instancia - {algoritmo}")
    plt.grid(True, which="both", ls="--", linewidth=0.5)
    plt.tight_layout()

    output_path = os.path.join(directory, f"cost_{algoritmo}.png") 
    plt.savefig(output_path)
    print(f"Chart saved as {output_path}")
    
    plt.close()  # Cierra la figura actual para no superponer