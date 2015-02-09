#!/bin/sh

ip=1

if [ "$(id -u)" != "0" ]; then
     echo "This script must be run as root" 1>&2
     exit 1
fi

while getopts cs option
 do
     case "${option}"
     in
         s) ip=1;;
         c) ip=2;;
     esac
 done


stop network-manager
modprobe batman-adv
ifconfig wlan0 down
ifconfig wlan0 mtu 1528
iwconfig wlan0 mode Ad-Hoc essid autosafe channel 4 key off
batctl if add wlan0
ifconfig bat0 up
ifconfig wlan0 up 
ifconfig bat0 10.0.0.$ip
