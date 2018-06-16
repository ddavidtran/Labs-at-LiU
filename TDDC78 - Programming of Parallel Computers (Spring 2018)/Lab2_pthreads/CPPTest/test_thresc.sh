#!/bin/bash

for n in {1..5}; do
	echo -e '\n' >> "result$i.txt"
	echo "Iteration: $n" >> "result$2.txt"
	./$1 "images/im$2.pbm" "test.pbm" >> "result$2.txt"
dones
