#!/bin/bash

for n in {1..5}; do
	echo "Iteration: $n \n" >> "result$2.txt"
	mpprun $1 $3 "images/im$2.pbm" "test.pbm" >> "result$2.txt"
done
