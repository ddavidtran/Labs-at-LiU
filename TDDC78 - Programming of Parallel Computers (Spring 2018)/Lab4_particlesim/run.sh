#!/bin/bash
set -e

for cores in {1,2,4,8,16}; do
    echo "Cores: $cores " >> "lab4_result.txt"
    salloc -N1 -n$cores mpprun ./particlesim $1  >> "lab4_result.txt"
    echo ""
done

echo "Cores: 32" >> "lab4_result.txt"
salloc -N2 -n32 mpprun ./particlesim $1 >> "lab4_result.txt"
