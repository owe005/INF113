#! usr/bin/bash

while :
do
    top -n 1 -p $(pgrep -d',' -f "$3") >> $2 #periodically check matching process argument 3, save log to argument 2
    sleep $1 #sleep for n seconds, n being argument 1
done

