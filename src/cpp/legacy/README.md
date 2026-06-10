# Legacy C++ source files

This directory contains selected legacy C++ source files from the original graph-matching implementation.

## Files

```text
ConsoleApplication1.cpp
Graph.h
ConsoleApplication1.vcxproj
ConsoleApplication1.vcxproj.filters
```

## Purpose

These files are released to improve transparency of the original graph construction, regional merging, and KM-based matching workflow.

The files are not a complete reproduction driver for all revised multi-seed experiments reported in Tables 2-5 of the manuscript. The revised manuscript adds a more detailed online simulation protocol, dummy no-assignment options, multi-seed statistical validation, component ablation, RH-KM comparison, movement-weight sensitivity analysis, and additional service-quality and movement-control metrics.

The full internal preprocessing and simulation codebase is not publicly released at this stage because it is part of an ongoing internal research codebase and is subject to data-use and laboratory management restrictions.

## Important notes

Some default constants and hard-coded input file paths in the legacy C++ files reflect original test settings. For example, the source file may read a legacy processed text file such as `../60-10.txt`, and default parameters may need to be adjusted when users run other settings.

Users should refer to the configuration files in `configs/` for the representative setting used in the revised manuscript.

## Excluded files

The following files are not included because they are user-local settings, IDE caches, compiled binaries, or build intermediates:

```text
.vs/
x64/
Debug/
ipch/
.suo
ConsoleApplication1.vcxproj.user
Browse.VC.db
*.obj
*.pdb
*.ilk
*.log
*.recipe
*.tlog
```