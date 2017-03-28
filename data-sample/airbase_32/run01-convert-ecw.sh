#!/bin/bash


wdir="$PWD"

find $wdir -name *.ecw | while read ll
do
    bn="${ll::-4}"
    fout="${bn}.tif"
    if [ -f "${fout}" ]; then
	echo "File [${fout}] exist, skip..."
    else
	gdal_translate "${ll}" "${fout}"
    fi
done
