#!/bin/sh

ip=1

if [ "$(id -u)" != "0" ]; then
     echo "ERROR: This script must be run as root" 1>&2
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

echo "Stopping Network Manager"
stop network-manager
if [ $? ]; then
    echo "($?)ERROR: Network Manager, did not exit cleanly"
    exit 1
fi

echo "Enabling the BAT-Signal"
modprobe batman-adv
if [ $? ]; then
    echo "($?)ERROR: BATMAN Kernel module did not load correctly"
    exit 1
fi

echo "Turning off Wlan0"
ifconfig wlan0 down
if [ $? ]; then
    echo "($?)ERROR: Wlan0 Did not shutdown"
    exit 1
fi

echo "Configuring MTU"
ifconfig wlan0 mtu 1528
if [ $? ]; then
    echo "($?)ERROR: MTU not set"
    exit 1
fi

echo "Enabling Ad-Hoc with SSID 'autosafe'"
iwconfig wlan0 mode Ad-Hoc essid autosafe channel 4 key off
if [ $? ]; then
    echo "($?)ERROR: Could not set Ad-hoc parameters"
    exit 1
fi

echo "Adding Wlan0 to BATMAN"
batctl if add wlan0
if [ $? ]; then
    echo "($?)ERROR: Wlan0 could not attach to bat0"
    exit 1
fi

echo "Enabling bat0 interface"
ifconfig bat0 up
if [ $? ]; then
    echo "($?)ERROR: Bat0 did not start"
    exit 1
fi

echo "Enabling Wlan0 interface"
ifconfig wlan0 up 
if [ $? ]; then
    echo "($?)ERROR: Wlan0 did not start"
    exit 1
fi

echo "Setting ip to $ip"
ifconfig bat0 10.0.0.$ip
if [ $? ]; then
    echo "($?)ERROR: ip Was not set"
    exit 1
fi
