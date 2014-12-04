**serial_spewer**
Serial spewer is is a script to read data via usb serial and write it to a file
The idea is to read the packets sent by the micro controller and record them as
test data later.

It takes the form
python serial_spewer.py <filename> <usbdevice>

where file name is the name of the file you want to write to and usb device is
the name of the usb device that is connected. This can be found in the lower
right corner of the arduino IDE and in linux takes the form "/dev/ttyACM0"

It will keep running until terminated with something like SIG\_INT
