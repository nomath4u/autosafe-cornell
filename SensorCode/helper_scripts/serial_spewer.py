##############################################################################
## This is a script to take the data taken from arduino serial and write it 
## to a file
################################################################################

from time import sleep
import serial
import argparse
import datetime
parser = argparse.ArgumentParser(description='Send arduino data to file')
parser.add_argument("filename", help="name of file to store data")
parser.add_argument("device", help="path to arduino device")
args = parser.parse_args()
print args.filename
print args.device

ser = serial.Serial(args.device, 9600) # Establish the connection on a specific port
data_file = open(args.filename, 'w') #Open the file, clobber existing
while True:
 out = ser.readline();
 data_file.write(out) # Read the newest output from the Arduino and send it to the file
 print out + str(datetime.datetime.now().time());
# data_file.write("Test\n")

