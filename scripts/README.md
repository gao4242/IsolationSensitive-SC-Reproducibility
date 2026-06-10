# Scripts

This directory provides example scripts for the released reproducibility package.

The scripts are designed to clarify the workflow used in the manuscript:

1. reading legacy processed text files;
2. generating semi-real online scenarios from fixed task records and simulated workers;
3. computing additional service-quality and movement-control metrics from assignment logs.

## Files

```text
prepare_data_from_txt.py
generate_online_scenarios.py
compute_metrics_from_assignment_log.py
```

## prepare_data_from_txt.py

This script reads legacy processed text files from `data/processed_txt/legacy/` and converts them into a unified CSV-style file.

Because the legacy txt files may use different field layouts, the script stores parsed values in generic fields such as `field_1`, `field_2`, and `field_3`.

Users should map the parsed fields to `task_id` and `region_id` according to the corresponding txt-file format before running `generate_online_scenarios.py`.

## generate_online_scenarios.py

This script generates semi-real online scenarios using fixed task records, simulated workers, worker availability slots, and task arrival slots.

The script assumes that each task has already been assigned to a regional node. Therefore, the input task CSV must contain a `region_id` column. The script does not infer task regions from row order.

## compute_metrics_from_assignment_log.py

This script computes additional service-quality and movement-control metrics from assignment logs.

The computed metrics include:

- completion rate;
- average waiting time;
- maximum waiting time;
- carry-over events;
- cross-region assignment ratio;
- average hop distance;
- worker-load standard deviation.

## Important note

These scripts are provided to document the experimental workflow and improve reproducibility. They do not replace the full internal research codebase.

The selected legacy C++ source files and the example Python scripts serve different purposes. The legacy C++ files show parts of the original graph-matching implementation. The Python scripts document the released semi-real scenario generation and metric-calculation workflow.