#!/bin/bash

# Usage is ./patching.sh folder_where_files_are

for f in $1/*; do
    # Will print */ if no directories are available
	echo ${f}/* ${f#*/}/
	# Uncomment the following to make a copy, its commented for now so I don't accidentally run this
	# rsync -a ${f}/* ${f#*/}/
done
