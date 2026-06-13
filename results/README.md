# Results directory

This directory is reserved for generated experimental outputs.

The revised manuscript reports the final results in the following tables and figures:

* Figures 4–9: parameter-effect experiments for objective value and running time;
* Table 2: multi-seed statistical validation under the representative parameter setting;
* Table 3: component ablation and stronger-baseline check;
* Table 4: movement-weight parameter sensitivity analysis;
* Table 5: service-quality and movement-control metrics.

The current supplementary package does not include the full per-seed internal assignment logs from the internal research codebase. Therefore, the repository does not provide complete regeneration of all final values reported in Figures 4–9 and Tables 2–5.

Users can generate new outputs by running the released scripts and selected implementation files. Generated outputs can be placed under:

```text
results/
└── generated/
    ├── table2_statistical_validation.csv
    ├── table3_ablation_and_rhkm.csv
    ├── table4_lambda_sensitivity.csv
    └── table5_additional_metrics.csv
```

The manuscript should be used as the authoritative source for the final reported numerical values. This directory is provided to clarify the expected output structure for users who run the released workflow scripts.
