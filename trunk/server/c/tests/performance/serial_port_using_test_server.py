#!/usr/bin/env python

import serial
import sys
import threading

baudrate = -1
try:
    baudrate = int(sys.argv[1])
except:
    print 'Command line error, syntax:'
    print '%s <baud rate> <portname>' % sys.argv[0]
    exit(-1)

portname = 0
if len(sys.argv) > 2:
    portname = sys.argv[2]

def assertEquals ( expected, actual ):
    if expected != actual:
        raise Exception('ASSERTION ERROR: expected "%s" but got "%s"' % (expected, actual))

class MyPort:
    def __init__(self, port, baudrate, parity = 'N', rtscts = False, xonxoff = False):
        self.serial = serial.Serial(port, baudrate, parity=parity, rtscts=rtscts, xonxoff=xonxoff, timeout=0.7)
        self.read_lines = []
        #self.write_lines = []
    
    def start(self):
        self.alive = True
        #start serial->console thread
        self.receiver_thread = threading.Thread(target=self.reader)
        self.receiver_thread.setDaemon(1)
        self.receiver_thread.start()
        
        #enter console->serial loop
        self.transmitter_thread = threading.Thread(target=self.writer)
        self.transmitter_thread.setDaemon(1)
        self.transmitter_thread.start()

    def stop(self):
        self.alive = False
        
    def join(self):
        self.transmitter_thread.join()
        self.receiver_thread.join()

    def reader(self):
        """loop and copy serial->console"""
        while self.alive:
            line = ''
            data = self.serial.read(1)

            if data == '\r':
                print 'READ:', line
                self.readlines.append ( line )
            else:
                line += data
    
    def writer(self):
        """loop and copy console->serial until EXITCHARCTER character is found"""
        while self.alive:
            try:
                c = '\n'
            except KeyboardInterrupt:
                c = '\x03'
            
            if c == '\n':
                self.serial.write('PROTOCOL\r\n')         # send newline character(s)
            else:
                self.serial.write(c)                    # send character
                if self.echo:
                    sys.stdout.write(c)

    def writeline ( self, line ):
        print 'WRITE:', line
        self.serial.write ( '%s\r\n' % line )
        self.serial.flush()

port = MyPort(portname, baudrate)
port.start()

#port = serial.Serial(portname, baudrate, timeout = 0.7)
#print 'Opened:', port.portstr
print port


def readline ():
    global port

#    line = ''
#    while True:
#        data = port.read(1)
#        
#        if data == '\r':
#            print 'READ:', line
#            return line
#
#        else:
#            line += data

def assertLine ( expected_line ):
    global port

    read_line = readline()
    assertEquals ( expected_line, read_line )

def writeline ( line ):
    global port

    port.write ( '%s\r\n' % line )

writeline ( 'PROTOCOL' )
assertLine ( '+PROTOCOL 1' )

print 'Target is running protocol version 1, OK!'
port.write ( 'LIST /type_tests/properties\r\n' )
properties_list = port.readline ()
print properties_list

port.close()
