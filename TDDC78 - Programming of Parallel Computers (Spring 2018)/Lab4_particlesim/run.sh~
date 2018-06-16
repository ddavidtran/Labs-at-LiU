#!/bin/bash
set -e

for cores in {1,2,4,8,16}; do
    echo "Cores: $cores " >> "lab3_result.txt"
    ompsalloc -N1 -c$cores ./myprogram >> "lab3_result.txt"
    echo ""
done
