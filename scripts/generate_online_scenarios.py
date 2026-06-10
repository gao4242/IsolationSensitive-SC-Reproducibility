"""
generate_online_scenarios.py

This script generates semi-real online scenarios for isolation-sensitive spatial
crowdsourcing experiments.

The task records are kept fixed. Worker records, worker locations, task arrival
slots, and worker availability slots are generated under each random seed.

The script follows the simulation protocol described in the manuscript:

1. real task locations are kept fixed;
2. each task should already have a region_id generated from real task coordinates
   and the selected regional partition;
3. worker number is generated according to the worker-task ratio;
4. worker regions are sampled from the same regional service space;
5. task arrival slots are sampled from the time-slot sequence;
6. worker availability slots are sampled from the time-slot sequence;
7. all algorithms should use the same generated scenario under the same seed.

This script does not implement the matching algorithms. It generates scenario files
that can be used by matching implementations.
"""

import argparse
import csv
import json
import math
import random
from pathlib import Path


def read_json(path):
    """Read a JSON configuration file."""
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def read_seeds(path):
    """Read integer seeds from a text file."""
    seeds = []
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            value = line.strip()
            if value:
                seeds.append(int(value))
    return seeds


def read_task_records(path):
    """Read task records from a CSV file.

    Required columns:
    - region_id

    Optional columns:
    - task_id
    - id

    If task_id is missing, a sequential task id is created.
    The region_id column is required because task regions should be generated
    from real task coordinates and the selected regional partition before this
    script is used.
    """
    tasks = []
    with open(path, "r", encoding="utf-8-sig", newline="") as f:
        reader = csv.DictReader(f)

        if reader.fieldnames is None:
            raise ValueError("The task CSV file has no header.")

        if "region_id" not in reader.fieldnames and "region" not in reader.fieldnames:
            raise ValueError(
                "The task CSV file must contain a region_id column. "
                "Please preprocess real task coordinates into regional nodes first."
            )

        for idx, row in enumerate(reader, start=1):
            task_id = row.get("task_id") or row.get("id") or f"q{idx}"
            region_id = row.get("region_id") or row.get("region") or ""

            tasks.append({
                "task_id": task_id,
                "region_id": region_id
            })

    return tasks


def ensure_task_regions(tasks, num_regions):
    """Validate task region ids.

    Each task should already contain a region_id generated from the real task
    coordinates and the selected regional partition. This script does not infer
    region ids from row order.
    """
    updated = []

    for task in tasks:
        region_id = task["region_id"]

        if region_id == "":
            raise ValueError(
                "Missing region_id. Please preprocess real task coordinates into "
                "regional nodes before generating online scenarios."
            )

        try:
            region_int = int(float(region_id))
        except ValueError as exc:
            raise ValueError(f"Invalid region_id: {region_id}") from exc

        if region_int < 0 or region_int >= num_regions:
            raise ValueError(
                f"region_id {region_id} is outside the valid range [0, {num_regions - 1}]."
            )

        updated.append({
            "task_id": task["task_id"],
            "region_id": str(region_int)
        })

    return updated


def generate_workers(num_workers, num_regions, rng):
    """Generate simulated workers."""
    workers = []

    for i in range(1, num_workers + 1):
        worker_id = f"w{i}"
        region_id = str(rng.randint(0, num_regions - 1))

        workers.append({
            "worker_id": worker_id,
            "region_id": region_id
        })

    return workers


def assign_task_arrivals(tasks, time_slots, rng):
    """Assign each task to an arrival slot."""
    rows = []

    for task in tasks:
        slot = rng.randint(1, time_slots)

        rows.append({
            "task_id": task["task_id"],
            "task_region": task["region_id"],
            "arrival_slot": slot
        })

    return rows


def assign_worker_availability(workers, time_slots, rng):
    """Assign each worker to one availability slot.

    If a worker may appear in multiple slots in another implementation, this
    function can be extended accordingly. This released script uses one sampled
    availability slot per worker to document the basic protocol.
    """
    rows = []

    for worker in workers:
        slot = rng.randint(1, time_slots)

        rows.append({
            "worker_id": worker["worker_id"],
            "worker_region": worker["region_id"],
            "available_slot": slot
        })

    return rows


def write_csv(rows, output_file, fieldnames):
    """Write rows to a CSV file."""
    output_path = Path(output_file)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    with open(output_path, "w", encoding="utf-8-sig", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()

        for row in rows:
            writer.writerow(row)


def main():
    parser = argparse.ArgumentParser(
        description="Generate semi-real online scenarios from fixed task records."
    )

    parser.add_argument(
        "--config",
        default="configs/representative_setting.json",
        help="Path to the representative setting JSON file."
    )

    parser.add_argument(
        "--tasks",
        default="outputs/task_region_records.csv",
        help=(
            "CSV file containing task records. The file must contain a region_id "
            "column generated from real task coordinates and regional partition."
        )
    )

    parser.add_argument(
        "--output-dir",
        default="outputs/scenarios",
        help="Output directory for generated scenarios."
    )

    args = parser.parse_args()

    config = read_json(args.config)
    params = config["parameters"]

    time_slots = int(params["time_slots"])
    num_regions = int(config["regional_partition"]["num_regional_nodes"])
    worker_task_ratio = float(params["worker_task_ratio"])

    seeds = read_seeds(config["random_seed_file"])

    tasks = read_task_records(args.tasks)
    tasks = ensure_task_regions(tasks, num_regions)

    num_workers = int(math.ceil(len(tasks) * worker_task_ratio))

    for seed in seeds:
        rng = random.Random(seed)

        workers = generate_workers(num_workers, num_regions, rng)
        task_arrivals = assign_task_arrivals(tasks, time_slots, rng)
        worker_availability = assign_worker_availability(workers, time_slots, rng)

        seed_dir = Path(args.output_dir) / f"seed_{seed}"
        seed_dir.mkdir(parents=True, exist_ok=True)

        write_csv(
            task_arrivals,
            seed_dir / "task_arrivals.csv",
            ["task_id", "task_region", "arrival_slot"]
        )

        write_csv(
            worker_availability,
            seed_dir / "worker_availability.csv",
            ["worker_id", "worker_region", "available_slot"]
        )

        write_csv(
            workers,
            seed_dir / "workers.csv",
            ["worker_id", "region_id"]
        )

        print(f"Generated scenario for seed {seed}: {seed_dir}")

    print("Scenario generation completed.")


if __name__ == "__main__":
    main()