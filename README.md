# Isolation-Sensitive Online Task Assignment in Spatial Crowdsourcing with Adaptive Regional Coarsening

This repository provides supplementary reproducibility materials for the manuscript:

**Isolation-Sensitive Online Task Assignment in Spatial Crowdsourcing with Adaptive Regional Coarsening**

The study investigates an isolation-sensitive online task assignment problem in spatial crowdsourcing. The service space is modeled as a regional adjacency graph. The assignment objective considers region-level movement cost, urgency reward, task carry-over, and online sequential decision-making.

The repository is intended to improve transparency of the dataset source, parameter settings, seed protocol, scenario-generation workflow, metric-calculation workflow, and selected original graph-matching implementation.

## Repository contents

```text
.
├── data/
│   ├── raw/
│   └── processed_txt/
│       └── legacy/
├── src/
│   └── cpp/
│       └── legacy/
├── configs/
├── scripts/
└── results/
```

## Data

The `data/raw/` directory contains the original task-related data files associated with the Changchun COVID-19-related scenario described in the manuscript.

The `data/processed_txt/legacy/` directory contains processed text files from the original implementation. The original file names are preserved to keep compatibility with the selected original source files.

In this repository, the folder name `legacy` refers to files inherited from the original implementation workflow. It does not mean that these files are invalid. These files are released to document the original processed input format and support workflow inspection.

## Source code

The `src/cpp/legacy/` directory contains selected original C++ source files from the original graph-matching implementation. These files are released to improve transparency of the original graph construction, regional merging, and KM-based matching workflow.

The selected original C++ source files are not a complete reproduction driver for all revised multi-seed experiments reported in Tables 2–5 of the manuscript. The revised experimental protocol is documented by the configuration files and example scripts in this repository.

Local IDE caches, user-specific Visual Studio files, compiled binaries, and build intermediates are not included because they are not required for understanding the experimental workflow and may contain local environment information.

## Configuration files

The `configs/` directory contains representative parameter settings and seed settings for the released reproducibility scripts.

Files:

```text
representative_setting.json
lambda_sensitivity_setting.json
seeds_30.txt
```

### representative_setting.json

This file records the representative parameter setting used for multi-seed statistical validation, component ablation, stronger-baseline check, and service-quality and movement-control metrics.

The representative setting follows the revised manuscript:

* time slots: 30
* initial coarsening threshold: 4
* coarsening threshold: 4
* number of regional nodes: 2500
* worker-task ratio: 1.2
* worker capacity: 3
* movement-weight parameter: 1
* dummy no-assignment weight: 0
* rolling-horizon length for RH-KM: 2

### lambda_sensitivity_setting.json

This file records the movement-weight parameter values used in the sensitivity analysis of lambda.

The tested values are:

```text
0.25
0.5
1.0
2.0
4.0
```

For each tested lambda, all compared algorithms should be rerun under the same generated online scenarios and the same seed protocol.

### seeds_30.txt

This file provides a fixed 30-seed protocol for the released reproducibility scripts.

The same seed should be used across all compared algorithms in each run so that the generated online scenario is paired across methods. This setting follows the multi-seed validation design described in the revised manuscript.

This seed file is provided for the released scripts. It should not be interpreted as a full internal per-seed assignment log from the original internal research codebase.

## Scripts

The `scripts/` directory provides example scripts for the released reproducibility package.

Files:

```text
prepare_data_from_txt.py
generate_online_scenarios.py
compute_metrics_from_assignment_log.py
```

### prepare_data_from_txt.py

This script reads processed text files from `data/processed_txt/legacy/` and converts them into a unified CSV-style file.

Because the processed text files may use different field layouts, the script stores parsed values in generic fields such as `field_1`, `field_2`, and `field_3`.

Users should map the parsed fields to `task_id` and `region_id` according to the corresponding text-file format before running `generate_online_scenarios.py`.

Example command:

```bash
python scripts/prepare_data_from_txt.py \
  --input-dir data/processed_txt/legacy \
  --output-file outputs/legacy_txt_records.csv
```

### generate_online_scenarios.py

This script generates semi-real online scenarios using fixed task records, simulated workers, worker availability slots, and task arrival slots.

The script assumes that each task has already been assigned to a regional node. Therefore, the input task CSV must contain a `region_id` column. The script does not infer task regions from row order.

The script follows the simulation protocol described in the manuscript:

1. real task locations are kept fixed;
2. each task should already have a `region_id` generated from real task coordinates and the selected regional partition;
3. worker number is generated according to the worker-task ratio;
4. worker regions are sampled from the same regional service space;
5. task arrival slots are sampled from the time-slot sequence;
6. worker availability slots are sampled from the time-slot sequence;
7. all compared algorithms should use the same generated scenario under the same seed.

Example command after preparing a task-region CSV file:

```bash
python scripts/generate_online_scenarios.py \
  --config configs/representative_setting.json \
  --tasks outputs/task_region_records.csv \
  --output-dir outputs/scenarios
```

This script does not implement the matching algorithms. It generates scenario files that can be used by matching implementations.

### compute_metrics_from_assignment_log.py

This script computes service-quality and movement-control metrics from assignment logs.

Expected input CSV columns:

```text
algorithm
seed
task_id
worker_id
arrival_slot
completion_slot
task_region
worker_region
hop_distance
carry_over_count
```

The computed metrics include:

* completion rate;
* average waiting time;
* maximum waiting time;
* carry-over events;
* cross-region assignment ratio;
* average hop distance;
* worker-load standard deviation.

Example command when compatible assignment logs are available:

```bash
python scripts/compute_metrics_from_assignment_log.py \
  --input-log outputs/assignment_log.csv \
  --output-file outputs/additional_metrics_summary.csv
```

This script can support the metric-calculation workflow used for the service-quality and movement-control metrics reported in Table 5 when compatible assignment logs are available.

## Results

The revised manuscript reports the final results in the following tables and figures:

* Figures 4–9: parameter-effect experiments for objective value and running time;
* Table 2: multi-seed statistical validation under the representative parameter setting;
* Table 3: component ablation and stronger-baseline check;
* Table 4: movement-weight parameter sensitivity analysis;
* Table 5: service-quality and movement-control metrics.

The `results/` directory is reserved for outputs generated by running the released scripts. Users can generate new outputs by running the released scripts and selected implementation files. Generated outputs can be placed under:

```text
results/
└── generated/
    ├── table2_statistical_validation.csv
    ├── table3_ablation_and_rhkm.csv
    ├── table4_lambda_sensitivity.csv
    └── table5_additional_metrics.csv
```

The current supplementary package does not include the full per-seed internal assignment logs from the internal research codebase. Therefore, the repository does not provide complete regeneration of all final values reported in Figures 4–9 and Tables 2–5.

The manuscript should be used as the authoritative source for the final reported numerical values. The `results/` directory is provided to clarify the expected output structure for users who run the released workflow scripts.

## Reproducibility scope

The released package supports transparency and partial reproducibility of the experimental workflow. It provides the data source, processed text files from the original implementation, selected original C++ source files, representative configuration files, a fixed 30-seed protocol, and example scripts for scenario generation and metric calculation.

The package supports the following components:

| Manuscript item                                   | Support provided in this repository                           |
| ------------------------------------------------- | ------------------------------------------------------------- |
| Dataset source                                    | Supported by `data/raw/`                                      |
| Processed inputs from the original implementation | Supported by `data/processed_txt/legacy/`                     |
| Representative parameter setting                  | Supported by `configs/representative_setting.json`            |
| Lambda-sensitivity setting                        | Supported by `configs/lambda_sensitivity_setting.json`        |
| Fixed seed protocol for released scripts          | Supported by `configs/seeds_30.txt`                           |
| Scenario-generation workflow                      | Supported by `scripts/generate_online_scenarios.py`           |
| Metric-calculation workflow                       | Supported by `scripts/compute_metrics_from_assignment_log.py` |
| Selected original graph-matching implementation   | Partially supported by `src/cpp/legacy/`                      |

The released package supports inspection and partial rerunning of several workflow components. For example, the configuration files document the representative and lambda-sensitivity settings, the seed file documents the 30-seed protocol, the scenario-generation script can generate semi-real online scenarios from fixed task records and simulated workers, and the metric-calculation script can compute service-quality and movement-control metrics when compatible assignment logs are available.

The package does not provide full regeneration of all reported figures and tables. Exact regeneration of Figures 4–9 and Tables 2–5 requires the complete internal implementation and full per-seed assignment logs. These materials are not publicly released because they are part of an ongoing internal research codebase and are subject to data-use and laboratory management restrictions.

Therefore, the manuscript should be used as the authoritative source for the final reported numerical values. The repository is intended to document the data source, parameter settings, seed protocol, selected implementation files, and released workflow scripts.

## Important notes

The experiments in the manuscript use real task locations and simulated worker availability and online arrivals. The released materials provide the original task-related data files, processed text files from the original implementation, selected original C++ source files, representative configuration files, a fixed 30-seed protocol for the released scripts, and example workflow scripts.

The full internal preprocessing and simulation codebase is not publicly released at this stage because it is part of an ongoing internal research codebase and is subject to data-use and laboratory management restrictions. To improve reproducibility within these constraints, the manuscript and this supplementary package provide the dataset source, regional partition settings, parameter settings, seed protocol, and statistical validation design.

## Environment

The Python example scripts use the Python standard library and do not require additional third-party Python packages.

The selected C++ source files are provided as original implementation files for workflow inspection. They may require a local C++ development environment to compile or run. Some default constants and hard-coded input file paths in the C++ files reflect original test settings, and users may need to adjust parameters and input paths when running other settings.

## Citation

If this repository is used, please cite the associated manuscript:

**Isolation-Sensitive Online Task Assignment in Spatial Crowdsourcing with Adaptive Regional Coarsening**
