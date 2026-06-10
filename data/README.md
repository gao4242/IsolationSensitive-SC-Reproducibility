# Data description

This directory contains the data files used or referenced in the manuscript.

## raw/

The `raw/` directory contains the original task-related data files associated with the Changchun COVID-19-related scenario.

Files:

```text
附件1：长春市COVID-19疫情期间病毒感染人数数据.xlsx
附表2：长春市9个区隔离人口数量与生活物资投放点数量.xlsx
附件3：长春市9个区交通网络数据和主要小区相关数据.xlsx
```

These files provide the original task-related data source used to construct the spatial task distribution in the study.

## processed_txt/legacy/

The `processed_txt/legacy/` directory contains legacy processed text files generated or used by the original implementation.

The original file names are preserved to keep compatibility with the selected legacy C++ source files. These files may correspond to different original test settings, intermediate processed inputs, or legacy parameter combinations.

Examples:

```text
data1.txt
data-1.txt
data-1-0.txt
data-1-2.txt
data2.txt
data-2.txt
data-2-0.txt
data-2-2.txt
30-10.txt
30-30.txt
30-50.txt
50-10.txt
50-30.txt
50-50.txt
60-10.txt
60-30.txt
60-50.txt
70-10.txt
70-20.txt
70-30.txt
70-50.txt
```

Because these files come from the original implementation workflow, they are released as legacy processed text files. Users should refer to the source files and scripts in this repository for how these files may be read.

## Important note

The experiments in the revised manuscript use real task locations and simulated worker availability and online arrivals. The released data files support transparency of the task-side data source and the legacy processed input format.