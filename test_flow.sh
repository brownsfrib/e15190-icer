#!/usr/bin/bash

. compile.sh Cumulant_Matrix.cxx
if [ $? -ne 0 ]; then
	echo -e "\033[1;31mFailed to compile - not executing033[0m\n"
else
	python3 python/submit_common_run.py \
		--projectile=ca40 \
		--target=ni58 \
		--energy=140 \
		--script=./bin/Cumulant_Matrix \
		--run=2632
fi
