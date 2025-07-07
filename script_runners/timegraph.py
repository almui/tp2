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
        if row['capacity_ok']==False:
            print(f"Skipping {row['instance']}, not valid.")
            continue
        num_items = clientes(row['instance'][1:])
        instancias[num_items]=row['execution_time_sec']


    results[filename.replace(".csv", "")] = instancias



# Crear DataFrame combinando todo
df_list = []
for algoritmo, tiempos in results.items():

    for num_items, tiempo in tiempos.items():
        df_list.append({
            "Algoritmo": algoritmo,
            "NumItems": num_items,
            "Tiempo": tiempo
        })

df = pd.DataFrame(df_list)

# Graficar cada algoritmo por separado 
plt.figure(figsize=(12, 7))
colores = ["#1f77b4",  "#ff7f0e","#2ca02c", "#d62728","#9467bd",  "#8c564b","#17becf"]
i=0
for algoritmo, grupo in df.groupby("Algoritmo"):
    grupo = grupo.sort_values("NumItems")
    
    plt.figure(figsize=(10, 6))  # Crea una nueva figura
    plt.plot(grupo["NumItems"], grupo["Tiempo"], marker='o', linestyle='--', label=algoritmo, color=colores[i])
    i=i+1
    
    plt.xlabel("Número de ítems")
    plt.ylabel("Tiempo de ejecución (s)")
    plt.title(f"Tiempos por instancia - {algoritmo}")
    plt.grid(True, which="both", ls="--", linewidth=0.5)
    plt.tight_layout()

    output_path = os.path.join(directory, f"time_{algoritmo}.png") 
    plt.savefig(output_path)
    print(f"Chart saved as {output_path}")
    
    plt.close()  # Cierra la figura actual para no superponer








