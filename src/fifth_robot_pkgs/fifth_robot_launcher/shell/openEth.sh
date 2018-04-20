#!/bin/sh

if [ $# -lt 1 ]; then
  target_device=` ifconfig | grep enp | cut -d' ' -f1 `
  echo "Device \"$target_device\" was DETECTED automatically."
else
  target_device=$1
  echo "Device \"$target_device\" was SELECTED with argument."
fi

ip_address="192.168.0.15"

sudo ifconfig $target_device $ip_address
sudo route add default gw 192.168.0.1
