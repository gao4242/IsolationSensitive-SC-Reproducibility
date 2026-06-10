# Configuration files

This directory contains configuration files for the representative experimental setting and the movement-weight sensitivity analysis.

## Files

```text
representative_setting.json
lambda_sensitivity_setting.json
seeds_30.txt
```

## representative_setting.json

This file records the representative parameter setting used for multi-seed statistical validation, component ablation, stronger-baseline check, and additional service-quality and movement-control metrics.

The representative setting follows the revised manuscript:

- time slots: 30
- initial coarsening threshold: 4
- coarsening threshold: 4
- number of regional nodes: 2500
- worker-task ratio: 1.2
- worker capacity: 3
- movement-weight parameter: 1
- dummy no-assignment weight: 0
- rolling-horizon length for RH-KM: 2

## lambda_sensitivity_setting.json

This file records the movement-weight parameter values used in the sensitivity analysis of lambda.

The tested values are:

```text
0.25
0.5
1.0
2.0
4.0
```

For each tested lambda, all algorithms should be rerun under the same generated online scenarios and the same seed protocol.

## seeds_30.txt

This file provides a fixed 30-seed protocol for the released reproducibility scripts.

The same seed should be used across all compared algorithms in each run so that the generated online scenario is paired across methods. This setting follows the multi-seed validation design described in the revised manuscript.

This seed file is provided for the released scripts. It should not be interpreted as a full internal per-seed assignment log from the original internal research codebase.