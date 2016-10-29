#!/bin/sh

sudo ifconfig $1 192.168.0.15
sudo route add default gw 192.168.0.1
