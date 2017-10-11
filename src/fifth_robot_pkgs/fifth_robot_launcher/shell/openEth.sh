#!/bin/sh

#!/bin/sh

if [ "$1" = "" ] ; then 
   echo "Input your Etherner device connected to Hokuyo"
   exit
fi

sudo ifconfig $1 192.168.0.15
sudo route add default gw 192.168.0.1

