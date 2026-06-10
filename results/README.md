# Results directory

This directory is reserved for generated experimental outputs.

The revised manuscript reports the final results in the following tables:

- Table 2: multi-seed statistical validation under the representative parameter setting;
- Table 3: component ablation and stronger-baseline check;
- Table 4: movement-weight parameter sensitivity analysis;
- Table 5: additional service-quality and movement-control metrics.

The current supplementary package does not include full per-seed internal assignment logs from the internal research codebase.

Users can generate new outputs by running the released scripts and selected implementation files. Generated outputs can be placed under:

```text
results/
└── generated/
    ├── table2_statistical_validation.csv
    ├── table3_ablation_and_rhkm.csv
    ├── table4_lambda_sensitivity.csv
    └── table5_additional_metrics.csv
```

The manuscript should be used as the authoritative source for the reported final table values.