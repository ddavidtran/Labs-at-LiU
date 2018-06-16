#!/bin/bash
set -e

for i in {1,4}; do
	for cores in {1,2,4,8,16}; do
	echo "Cores: $cores " >> "result$i.txt"
	echo -e '\n' >> "result$i.txt"
	salloc -N1 -n$cores sh test_blurc.sh $1 $i $2
	done
done
