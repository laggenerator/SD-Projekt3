#!/bin/bash

# Usage: ./csv_to_latex.sh input.csv output.tex [num_lines]

INPUT_FILE="${1:-input.csv}"
OUTPUT_FILE="${2:-output.tex}"
NUM_LINES="${3:-10}"

# Process the CSV and format for LaTeX
awk -F';' -v num_lines="$NUM_LINES" '
BEGIN {
    srand()
    # LaTeX special character escaping
    gsub("[\\&%_]", "\\\\&", header)
}
NR == 1 {
    header = $0
    next
}
{
    # Store all rows for random selection
    rows[NR-2] = $0
}
END {
    # Select random lines if we have enough
    total_rows = length(rows)
    if (num_lines > total_rows) num_lines = total_rows
    
    # Create array of random indices
    for (i = 0; i < num_lines; i++) {
        indices[i] = i
    }
    if (num_lines < total_rows) {
        for (i = num_lines; i < total_rows; i++) {
            j = int(rand() * (i + 1))
            if (j < num_lines) {
                indices[j] = i
            }
        }
    }
    
    # Process selected rows
    for (i = 0; i < num_lines; i++) {
        split(rows[indices[i]], fields, ";")
        
        # Format numbers (remove .0 for integers, keep 2 decimals otherwise)
        for (j = 1; j <= 4; j++) {
            if (fields[j] ~ /^[0-9]+\.0+$/) {
                sub(/\.0+$/, "", fields[j])
            }
            else if (fields[j] ~ /^[0-9]+\.[0-9]+$/) {
                fields[j] = sprintf("%.2f", fields[j])
                sub(/0+$/, "", fields[j])
                sub(/\.$/, "", fields[j])
            }
        }
        printf "        %s & %s & %s & %s \\\\ \\hline\n", fields[1], fields[2], fields[3], fields[4]
    }
}' "$INPUT_FILE" | sort -t '&' -k1,1n > "$OUTPUT_FILE"

echo "Formatted ${NUM_LINES} random rows into LaTeX table format in ${OUTPUT_FILE}"
