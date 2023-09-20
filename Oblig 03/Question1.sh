#! usr/bin/bash

mkdir -pv "$1" #create folder for argument

cd $1 #cd into said folder

for i in $(seq 1 $2); do #for n times
  mkdir -pv "Question$i" #create folder+"n"
done

echo "You just created $2 folders within $1"
exit

