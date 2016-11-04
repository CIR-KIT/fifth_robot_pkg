#!/bin/sh

if [ $# -lt 1 ]; then
  echo "I need eth name"
  echo "$0 <eth name>"
  return 1
fi

sudo ifconfig $1 192.168.0.15
sudo route add default gw 192.168.0.1
