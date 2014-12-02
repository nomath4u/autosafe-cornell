##############################################################################
## This is a script to take the data taken from arduino serial and write it 
## to a file
################################################################################

from time import sleep
import serial
import argparse
parser = argparse.ArgumentParser()
#ser = serial.Serial('/dev/tty.usbmodem1d11', 9600) # Establish the connection on a specific port
data_file = open('data.txt', 'w') #Open the file, clobber existing
while True:
# data_file.write(ser.readline()) # Read the newest output from the Arduino and send it to the file
  data_file.write("Test\n")
  sleep(.1) # Delay for one tenth of a second

