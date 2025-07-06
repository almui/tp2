import os
import subprocess
import csv
import time

# Paths
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, ".."))
INSTANCE_DIR = os.path.join(PROJECT_ROOT, "instancias")
BINARY_PATH = os.path.join(PROJECT_ROOT, "src", "vrp")

# Config
ALGORITHMS = ["clarke_wright", "insertion"]
LOCAL_SEARCH= ["relocate", "swap"]
OUTPUT_FIELDS = ["instance", "execution_time_sec", "total_cost", "capacity_ok"]

def get_max_vehicles(instance_path):
    with open(instance_path, 'r') as f:
        for line in f:
            if line.strip().lower().startswith("vehicles"):
                try:
                    return int(line.strip().split()[-1])
                except ValueError:
                    pass
    return None

def parse_output(output, max_vehicles):
    total_cost = None
    vehicle_limit_respected = False

    for line in output.splitlines():
        line = line.strip()
        if line.startswith("TOTAL DISTANCE TRAVELED:"):
            try:
                total_cost = float(line.split(":")[1].strip())
            except ValueError:
                pass
        if line.startswith("ROUTES :"):
            try:
                num_routes = int(line.split(":")[1].strip())
                if max_vehicles is not None:
                    vehicle_limit_respected = num_routes <= max_vehicles
            except ValueError:
                pass

    return total_cost, vehicle_limit_respected

def run_benchmark():
    for algo in ALGORITHMS:
        for lsearch in LOCAL_SEARCH:
            results = []

            for fname in os.listdir(INSTANCE_DIR):
                if not fname.endswith(".dat"):
                    continue

                instance_path = os.path.join(INSTANCE_DIR, fname)
                max_vehicles = get_max_vehicles(instance_path)
                swap= lsearch=="swap"
                relocate= lsearch=="relocate"
                cmd = [BINARY_PATH, instance_path, algo, str(swap), str(relocate)]


                print(f"Running {algo} + {lsearch} on {fname}...")

                start = time.time()
                try:
                    result = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
                except subprocess.TimeoutExpired:
                    print(f"Timeout on {fname}")
                    continue
                end = time.time()

                output = result.stdout + result.stderr
                cost, capacity_ok = parse_output(output, max_vehicles)

                results.append({
                    "instance": fname,
                    "execution_time_sec": round(end - start, 4),
                    "total_cost": cost if cost is not None else "N/A",
                    "capacity_ok": capacity_ok
                })

            out_path = os.path.join(SCRIPT_DIR, f"{algo}+{lsearch}.csv")
            with open(out_path, "w", newline="") as f:
                writer = csv.DictWriter(f, fieldnames=OUTPUT_FIELDS)
                writer.writeheader()
                writer.writerows(results)

            print(f"Saved results to {out_path}\n")

def run_allbenchmark():

    results = []

    for fname in os.listdir(INSTANCE_DIR):
        if not fname.endswith(".dat"):
            continue

        instance_path = os.path.join(INSTANCE_DIR, fname)
        max_vehicles = get_max_vehicles(instance_path)
        cmd = [BINARY_PATH, instance_path, "clarke_wright", "True", "True"]


        print(f"Running clarke_wright + relocate + swap on {fname}...")

        start = time.time()
        try:
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
        except subprocess.TimeoutExpired:
            print(f"Timeout on {fname}")
            continue
        end = time.time()

        output = result.stdout + result.stderr
        cost, capacity_ok = parse_output(output, max_vehicles)

        results.append({
            "instance": fname,
            "execution_time_sec": round(end - start, 4),
            "total_cost": cost if cost is not None else "N/A",
            "capacity_ok": capacity_ok
        })

    out_path = os.path.join(SCRIPT_DIR, f"clarke_wright+relocate+swap.csv")
    with open(out_path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=OUTPUT_FIELDS)
        writer.writeheader()
        writer.writerows(results)

    print(f"Saved results to {out_path}\n")


if __name__ == "__main__":
    run_benchmark()
    run_allbenchmark()
