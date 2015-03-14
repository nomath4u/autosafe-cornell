#! /usr/bin/env python

# Client and server for udp (datagram) echo.
#
# Usage: udpecho -s [port]            (to start a server)
# or:    udpecho -c host [port] <file (client)

import sys
from socket import *
import struct
import time
from optparse import OptionParser

ECHO_PORT = 50000 + 7
BUFSIZE = 1024
#filehandle = sys.argv[2]
gravity = 9.8 #because it comes out in m/s^2 and we want G's

class Packet():
	acc_x = 0.0
	acc_y = 0.0
	acc_z = 0.0
	ret_acc_x = 0.0
	ret_acc_y = 0.0
	ret_acc_z = 0.0
	velocity = 0.0
	rpm = 0.0
	
	def __init__(self, tup):
		self.acc_x = tup[0]/gravity
		self.acc_y = tup[1]/gravity
		self.acc_z = tup[2]/gravity
		self.ret_acc_x = tup[3]
		self.ret_acc_y = tup[4]
		self.ret_acc_z = tup[5]
		self.velocity = tup[6]
		self.rpm = tup[7]

	def print_packet(self):
		print ("{0},{1},{2},{3},{4},{5},{6},{7}".format(self.acc_x, self.acc_y, self.acc_z, self.ret_acc_x, self.ret_acc_y, self.ret_acc_z, self.velocity, self.rpm))

	def write_packet(self,options):
		f = open(options.filename, 'a+')
		f.write("{0},{1},{2},{3},{4},{5}".format(self.acc_x, self.acc_y, self.acc_z, self.ret_acc_x, self.ret_acc_y, self.ret_acc_z))
		#Need to put the velocity and rpm in the right spots.
		f.write(",0,0,0,0,0,0,0,0,0,0,0,0,0\n")
		f.close()
	
	def send_packet(self, options):
		out_socket = socket(AF_INET, SOCK_DGRAM)
		out_port = 7001
		out_socket.bind(('', out_port))
		#out_socket.sendto("HI!", (options.host_address,int(options.out_port)))
		out_socket.sendto(("{0},{1},{2},{3},{4},{5}".format(self.acc_x, self.acc_y, self.acc_z, self.ret_acc_x, self.ret_acc_y, self.ret_acc_z)),(options.host_address,int(options.out_port)))

def main():
	parser = OptionParser()
	parser.add_option("-l", "--listen", dest="listen_port", help="Port to listen for telemetry on")
	parser.add_option("-f", "--file", dest="filename", help="File you want to write data out to")
	parser.add_option("-H", "--host", dest="host_address", help="Host address you want to send formatted packet to")
	parser.add_option("-p", "--port", dest="out_port", help="The port on the host you want to send formatted packet to")
	(options, args) = parser.parse_args()
	check_args(options, args, parser)
	server(options)

def usage():
	sys.stdout = sys.stderr
	print 'Usage: ' + sys.argv[0] + ' <port> <filehandle>'
	sys.exit(2)

def check_args(options, args, parser):
	if(args):
		parser.error('Unrecognized options')
		exit(2)
	if not options.listen_port or not options.filename or not options.host_address or not options.out_port:
		parser.error('Missing option(s)')
		exit(2)
	


def server(options):
	s = socket(AF_INET, SOCK_DGRAM)
	port = int(options.listen_port)
	s.bind(('', port))
	print 'udp logger server ready'
	old_time = time.time()
	new_time = time.time()
	while 1:
		data, addr = s.recvfrom(BUFSIZE)
		new_time = time.time()
		if(new_time - old_time >= .1): #It has been as close to 1 second as we can get
			packet = Packet(struct.unpack('ffffffff', data))
			packet.print_packet()
			#packet.write_packet(options)
			#packet.send_packet(options)
			old_time = new_time

main()
