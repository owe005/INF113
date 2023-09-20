#! usr/bin/bash

while :
do
    top -n 1 >> $2 #save to output file, argument 2
    sleep $1 #sleep for n seconds, n being argument 1
done


