#!/bin/sh

sudo ifconfig enp3s0 192.168.0.15
sudo route add default gw 192.168.0.1
