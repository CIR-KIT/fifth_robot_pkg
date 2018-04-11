#!/bin/sh

# if [ $# -lt 1 ]; then
#   echo "AutoDetect-"
#   target=` ifconfig | grep enp | cut -d' ' -f1 `
#   echo $target
# else 
#   target=$1
# fi


ip_address="192.168.0.15"

sudo ifconfig enp0s31f6 $ip_address
sudo route add default gw 192.168.0.1
