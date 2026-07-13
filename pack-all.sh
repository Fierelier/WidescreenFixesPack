#!/usr/bin/env sh
set -e

cd data
for SUITE in *; do
	if ! [ -d "$SUITE" ]; then continue; fi
	if [ -e "$SUITE/scripts"/*.asi ]; then
		cd "$SUITE"
		ZIP="../../$SUITE.zip"
		if [ -e "$ZIP" ]; then
			rm "$ZIP"
		fi
		zip -r "$ZIP" "scripts"
		cd ..
	fi
done
