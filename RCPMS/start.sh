#!/bin/bash -v
filename=$1
for i in {1..1}
do
	echo "Execution round $i "
	while read line; do
	# reading each line
		echo "$line"
		./main $line --TEMP_INIT 0.1 --TEMP_FIM 0.5 --N_REPLICAS 20 --MCL 500 --PTL 400 --TEMP_DIST 3 --MOV_TYPE 1 --MOV_FORCE 1 --TYPE_UPDATE 1 --TEMP_UPDATE 50
	done < $filename
done
