#!/bin/sh

stop network-manager
modprobe batman-adv
ifconfig wlan0 down
ifconfig wlan0 mtu 1528
iwconfig wlan0 mode Ad-Hoc essid autosafe channel 4 key off
ifconfig bat0 up
ifconfig wlan0 up 
ifconfig bat0 10.0.0.1
