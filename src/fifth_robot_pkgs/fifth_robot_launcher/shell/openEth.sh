#!/bin/sh

if [ $# -lt 1 ]; then
  echo "AutoDetect-"
  target_device=` ifconfig | grep enp | cut -d' ' -f1 `
else
  echo "Device \"$target_device\" was selected-"
  target_device=$1
fi

ip_address="192.168.0.15"

sudo ifconfig $target_device $ip_address
sudo route add default gw 192.168.0.1
echo -e "\e[32;1mSucceeded in Ethernet connection to $target_device\e[m"
