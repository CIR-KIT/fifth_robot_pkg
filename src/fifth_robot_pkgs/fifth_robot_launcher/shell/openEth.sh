#!/bin/bash

if [ $# -lt 1 ]; then
  target_device=` ifconfig | grep enp | cut -d' ' -f1 `
  echo "Device \"$target_device\" was DETECTED automatically."
else
  target_device=$1
  echo "Device \"$target_device\" was SELECTED with argument."
fi

ip_address="192.168.0.15"
nmcli dev disconnect $target_device
sudo ifconfig $target_device $ip_address
sudo route add default gw 192.168.0.1

if [ $? -eq 0 ]; then
    echo -e "\e[32;1mSucceeded in Ethernet connection to $target_device\e[m"
else
    echo -e "\e[31;1mFailed in Ethernet connection to $target_device\e[m"
fi
