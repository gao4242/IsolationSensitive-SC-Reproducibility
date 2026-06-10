
"""
compute_metrics_from_assignment_log.py

This script computes additional service-quality and movement-control metrics from
assignment logs.

Expected input CSV columns:

- algorithm
- seed
- task_id
- worker_id
- arrival_slot
- completion_slot
- task_region
- worker_region
- hop_distance
- carry_over_count

The script computes:

1. completion rate;
2. average waiting time;
3. maximum waiting time;
4. carry-over events;
5. cross-region assignment ratio;
6. average hop distance;
7. worker-load standard deviation.

This script is intended to support the additional metric analysis reported in the
manuscript.
"""

import argparse
import csv
import math
from collections import defaultdict
from pathlib import Path


def to_float(value, default=None):
    try:
        if value is None or value == "":
            return default
        return float(value)
    except ValueError:
        return default


def to_int(value, default=None):
    try:
        if value is None or value == "":
            return default
        return int(float(value))
    except ValueError:
        return default


def read_assignment_log(path):
    rows = []
    with open(path, "r", encoding="utf-8-sig", newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            rows.append(row)
    return rows


def group_by_algorithm_seed(rows):
    grouped = defaultdict(list)
    for row in rows:
        algorithm = row.get("algorithm", "unknown")
        seed = row.get("seed", "0")
        grouped[(algorithm, seed)].append(row)
    return grouped


def std(values):
    if not values:
        return 0.0
    mean = sum(values) / len(values)
    variance = sum((x - mean) ** 2 for x in values) / len(values)
    return math.sqrt(variance)


def compute_one_run(rows):
    total_tasks = len(rows)
    completed_rows = []

    for row in rows:
        worker_id = row.get("worker_id", "")
        completion_slot = to_int(row.get("completion_slot"), None)
        if worker_id not in ["", "dummy", "none", "NA"] and completion_slot is not None:
            completed_rows.append(row)

    completed_tasks = len(completed_rows)
    completion_rate = 100.0 * completed_tasks / total_tasks if total_tasks > 0 else 0.0

    waiting_times = []
    hop_distances = []
    cross_region_count = 0
    carry_over_events = 0
    worker_load = defaultdict(int)

    for row in completed_rows:
        arrival_slot = to_int(row.get("arrival_slot"), None)
        completion_slot = to_int(row.get("completion_slot"), None)

        if arrival_slot is not None and completion_slot is not None:
            waiting_times.append(max(0, completion_slot - arrival_slot))

        hop_distance = to_float(row.get("hop_distance"), None)
        if hop_distance is None:
            task_region = row.get("task_region", "")
            worker_region = row.get("worker_region", "")
            hop_distance = 0.0 if task_region == worker_region else 1.0

        hop_distances.append(hop_distance)

        if hop_distance > 0:
            cross_region_count += 1

        carry_count = to_int(row.get("carry_over_count"), 0)
        carry_over_events += carry_count

        worker_id = row.get("worker_id", "")
        if worker_id not in ["", "dummy", "none", "NA"]:
            worker_load[worker_id] += 1

    avg_waiting_time = sum(waiting_times) / len(waiting_times) if waiting_times else 0.0
    max_waiting_time = max(waiting_times) if waiting_times else 0.0
    cross_region_ratio = 100.0 * cross_region_count / completed_tasks if completed_tasks > 0 else 0.0
    avg_hop_distance = sum(hop_distances) / len(hop_distances) if hop_distances else 0.0
    worker_load_std = std(list(worker_load.values()))

    return {
        "completion_rate": completion_rate,
        "avg_waiting_time": avg_waiting_time,
        "max_waiting_time": max_waiting_time,
        "carry_over_events": carry_over_events,
        "cross_region_ratio": cross_region_ratio,
        "avg_hop_distance": avg_hop_distance,
        "worker_load_std": worker_load_std
    }


def summarize_by_algorithm(run_metrics):
    by_algorithm = defaultdict(list)

    for (algorithm, seed), metrics in run_metrics.items():
        by_algorithm[algorithm].append(metrics)

    summary_rows = []

    metric_names = [
        "completion_rate",
        "avg_waiting_time",
        "max_waiting_time",
        "carry_over_events",
        "cross_region_ratio",
        "avg_hop_distance",
        "worker_load_std"
    ]

    for algorithm, metrics_list in sorted(by_algorithm.items()):
        row = {"algorithm": algorithm}
        for metric in metric_names:
            values = [m[metric] for m in metrics_list]
            mean_value = sum(values) / len(values) if values else 0.0
            std_value = std(values)
            row[f"{metric}_mean"] = mean_value
            row[f"{metric}_std"] = std_value
        summary_rows.append(row)

    return summary_rows


def write_summary(rows, output_file):
    fieldnames = [
        "algorithm",
        "completion_rate_mean",
        "completion_rate_std",
        "avg_waiting_time_mean",
        "avg_waiting_time_std",
        "max_waiting_time_mean",
        "max_waiting_time_std",
        "carry_over_events_mean",
        "carry_over_events_std",
        "cross_region_ratio_mean",
        "cross_region_ratio_std",
        "avg_hop_distance_mean",
        "avg_hop_distance_std",
        "worker_load_std_mean",
        "worker_load_std_std"
    ]

    output_path = Path(output_file)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    with open(output_path, "w", encoding="utf-8-sig", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        for row in rows:
            writer.writerow(row)


def main():
    parser = argparse.ArgumentParser(
        description="Compute service-quality and movement-control metrics from assignment logs."
    )
    parser.add_argument(
        "--input-log",
        required=True,
        help="Assignment log CSV file."
    )
    parser.add_argument(
        "--output-file",
        default="outputs/additional_metrics_summary.csv",
        help="Output summary CSV file."
    )

    args = parser.parse_args()

    rows = read_assignment_log(args.input_log)
    grouped = group_by_algorithm_seed(rows)

    run_metrics = {}
    for key, group_rows in grouped.items():
        run_metrics[key] = compute_one_run(group_rows)

    summary_rows = summarize_by_algorithm(run_metrics)
    write_summary(summary_rows, args.output_file)

    print(f"Processed assignment log: {args.input_log}")
    print(f"Output summary: {args.output_file}")


if __name__ == "__main__":
    main()