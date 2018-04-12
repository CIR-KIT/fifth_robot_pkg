#!/bin/sh

if [ $# -lt 1 ]; then
  echo "AutoDetect-"
  target=` ifconfig | grep enp | cut -d' ' -f1 `
  echo $target
else
  echo "Device \"$1\" was selected-"
  target=$1
  echo $target
fi

ip_address="192.168.0.15"

sudo ifconfig $target $ip_address
sudo route add default gw 192.168.0.1
