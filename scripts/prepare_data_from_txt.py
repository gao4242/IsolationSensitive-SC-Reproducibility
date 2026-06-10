"""
prepare_data_from_txt.py

This script reads legacy processed text files and converts them into a unified CSV file.

The original implementation used multiple text files with preserved file names.
Because the internal text-file format may vary across files, this script reads each
non-empty line, splits it by comma or whitespace, and stores the parsed fields in a
generic tabular format.

This script is provided as a transparent data-loading example for the released
reproducibility package.
"""

import argparse
import csv
import os
import re
from pathlib import Path


def parse_line(line):
    """Split one line by comma or whitespace."""
    line = line.strip()
    if not line:
        return []
    parts = re.split(r"[,\s]+", line)
    return [p for p in parts if p != ""]


def collect_txt_files(input_dir):
    """Collect all .txt files under the input directory."""
    input_path = Path(input_dir)
    files = sorted(input_path.rglob("*.txt"))
    return files


def read_legacy_txt_files(input_dir):
    """Read all legacy txt files and return generic rows."""
    rows = []
    txt_files = collect_txt_files(input_dir)

    for file_path in txt_files:
        with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
            for row_id, line in enumerate(f, start=1):
                parts = parse_line(line)
                if not parts:
                    continue

                row = {
                    "source_file": str(file_path),
                    "source_name": file_path.name,
                    "row_id": row_id
                }

                for idx, value in enumerate(parts, start=1):
                    row[f"field_{idx}"] = value

                rows.append(row)

    return rows


def write_csv(rows, output_file):
    """Write rows to CSV with dynamic field names."""
    if not rows:
        raise ValueError("No rows were parsed from the input directory.")

    fieldnames = set()
    for row in rows:
        fieldnames.update(row.keys())

    fixed_fields = ["source_file", "source_name", "row_id"]
    dynamic_fields = sorted([f for f in fieldnames if f not in fixed_fields])
    header = fixed_fields + dynamic_fields

    output_path = Path(output_file)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    with open(output_path, "w", encoding="utf-8-sig", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=header)
        writer.writeheader()
        for row in rows:
            writer.writerow(row)


def main():
    parser = argparse.ArgumentParser(
        description="Read legacy processed txt files and convert them to a unified CSV file."
    )
    parser.add_argument(
        "--input-dir",
        default="data/processed_txt/legacy",
        help="Directory containing legacy .txt files."
    )
    parser.add_argument(
        "--output-file",
        default="outputs/legacy_txt_records.csv",
        help="Output CSV file."
    )

    args = parser.parse_args()

    rows = read_legacy_txt_files(args.input_dir)
    write_csv(rows, args.output_file)

    print(f"Parsed rows: {len(rows)}")
    print(f"Output file: {args.output_file}")


if __name__ == "__main__":
    main()