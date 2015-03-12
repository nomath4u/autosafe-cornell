#! /usr/bin/env python

# Client and server for udp (datagram) echo.
#
# Usage: udpecho -s [port]            (to start a server)
# or:    udpecho -c host [port] <file (client)

import sys
from socket import *
import struct
import time

ECHO_PORT = 50000 + 7
BUFSIZE = 1024
filehandle = sys.argv[2]
gravity = 9.8 #because it comes out in m/s^2 and we want G's

class Packet():
	acc_x = 0.0
	acc_y = 0.0
	acc_z = 0.0
	ret_acc_x = 0.0
	ret_acc_y = 0.0
	ret_acc_z = 0.0
	
	def __init__(self, tup):
		self.acc_x = tup[0]/gravity
		self.acc_y = tup[1]/gravity
		self.acc_z = tup[2]/gravity
		self.ret_acc_x = tup[3]
		self.ret_acc_y = tup[4]
		self.ret_acc_z = tup[5]

	def print_packet(self):
		print ("{0},{1},{2},{3},{4},{5}".format(self.acc_x, self.acc_y, self.acc_z, self.ret_acc_x, self.ret_acc_y, self.ret_acc_z))

	def write_packet(self):
		f = open(filehandle, 'a+')
		f.write("{0},{1},{2},{3},{4},{5}".format(self.acc_x, self.acc_y, self.acc_z, self.ret_acc_x, self.ret_acc_y, self.ret_acc_z))
		f.write(",0,0,0,0,0,0,0,0,0,0,0,0,0\n")
		f.close()

def main():
	if len(sys.argv) < 3:
		usage()
	#filehandle = sys.argv[2]
	server()

def usage():
	sys.stdout = sys.stderr
	print 'Usage: ' + sys.argv[0] + ' <port> <filehandle>'
	sys.exit(2)


def server():
	if len(sys.argv) > 1:
		port = eval(sys.argv[1])
	else:
		usage()
	s = socket(AF_INET, SOCK_DGRAM)
	s.bind(('', port))
	print 'udp logger server ready'
	old_time = time.time()
	new_time = time.time()
	while 1:
		data, addr = s.recvfrom(BUFSIZE)
		new_time = time.time()
		if(new_time - old_time >= .1): #It has been as close to 1 second as we can get
			packet = Packet(struct.unpack('ffffff', data))
			packet.print_packet()
			packet.write_packet()
			old_time = new_time

main()
