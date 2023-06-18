#!/bin/bash

i=1;
for modified in "$@"
do
    echo "copying - $modified";
	rsync -R $modified to_copy/
    i=$((i + 1));
done
