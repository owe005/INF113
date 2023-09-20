#! usr/bin/bash

mkdir -pv "$1" #create folder for keyword

for f in $(find . -type f -print) #for every file found
    do
        if grep -q "$1" "$f"; then #if match
        cp "$f" "$1/" #copy matching files to argument folder
        fi 
done

echo "You created a new folder $1 and moved all files containing $1 to $1"

