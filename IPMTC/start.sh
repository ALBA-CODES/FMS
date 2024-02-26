#!/bin/bash
filename=$1
for i in {1..1}
do
	echo "Execution round $i "
	while read line; do
	# reading each line
		echo "./main $line "
		./main $line
	done < $filename
done
