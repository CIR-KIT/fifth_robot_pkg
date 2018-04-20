#!/bin/bash

sudo chmod 777 /dev/serial/by-id/usb-T-frog_project_T-frog_Driver-if00

if [ $? -eq 0]; then
    echo -e "\e[32;1mSucceeded in USB connection to yp-spur\e[m"
else
    echo -e "\e[31;1mFailed in USB connection to yp-spur\e[m"
fi
