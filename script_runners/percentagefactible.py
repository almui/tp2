import os
import pandas as pd
import matplotlib.pyplot as plt

# Path to CSVs
directory = "script_runners"

# Get all CSV files
csv_files = [f for f in os.listdir(directory) if f.endswith(".csv")]

results = {}

for filename in csv_files:
    filepath = os.path.join(directory, filename)
    df = pd.read_csv(filepath)

    if "capacity_ok" not in df.columns:
        print(f"Skipping {filename}, 'capacity_ok' column not found.")
        continue

    # Normalize 'capacity_ok' to boolean
    df['capacity_ok'] = df['capacity_ok'].astype(str).str.strip().str.lower() == "true"

    total = len(df)
    valid = df['capacity_ok'].sum()
    percentage = (valid / total) * 100 if total > 0 else 0

    results[filename.replace(".csv", "")] = percentage

# Separate and order results
insertion_results = {k.replace("insertion", "insertion"): v for k, v in results.items() if k.startswith("insertion")}
cw_results = {k.replace("clarke_wright", "cw"): v for k, v in results.items() if k.startswith("clarke_wright")}

# Combine
ordered_labels = list(insertion_results.keys()) + list(cw_results.keys())
ordered_percentages = list(insertion_results.values()) + list(cw_results.values())

# Darker colors
insertion_color = '#4F81BD'
cw_color = '#9B59B6'
colors = [insertion_color] * len(insertion_results) + [cw_color] * len(cw_results)

# Plotting
plt.figure(figsize=(16, 6))  # Wider to avoid overlap
bars = plt.bar(ordered_labels, ordered_percentages, color=colors)

plt.ylabel("Percentage of Capacity OK (%)")
plt.title("Percentage of Solutions Meeting Capacity Constraint")
plt.xticks(rotation=0, ha="center", fontsize=10)  # Keep labels horizontal
plt.ylim(0, 100)
plt.tight_layout()
plt.grid(axis='y', linestyle='--', alpha=0.7)

# Optional: add labels above bars
for bar in bars:
    height = bar.get_height()
    plt.text(bar.get_x() + bar.get_width() / 2, height + 1, f'{height:.1f}%', ha='center', va='bottom', fontsize=8)

# Save
output_path = os.path.join(directory, "percentagefactible.png")
plt.savefig(output_path)
print(f"Chart saved as {output_path}")
