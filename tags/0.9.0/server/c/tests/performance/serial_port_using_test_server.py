#!/usr/bin/env python

import serial
import sys
import threading
import time
from datetime import datetime

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
    def __init__(self, port, baudrate, parity = 'N', stopbits = 1, rtscts = False, xonxoff = False):
        print port, baudrate
        self.serial = serial.Serial(port, baudrate, parity=parity, stopbits=stopbits, rtscts=rtscts, xonxoff=xonxoff, timeout=0.7)
        self.read_lines = []
        self.reader_data_available = threading.Condition()
        
        #self.read_lines_lock = threading.Semaphore()
        self.write_lines = []
        self.writer_data_avilable = threading.Condition()
    
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
        
        import time
        time.sleep ( 1 )
        
    def stop(self):
        self.alive = False
        
    def join(self):
        self.transmitter_thread.join()
        self.receiver_thread.join()

    def reader(self):
        """loop and copy serial->console"""
        line = ''
        
        while self.alive:
            data = self.serial.read(1)
            
            if data == '\r':
                print 'READ:', line
                self.reader_data_available.acquire()
                
                self.read_lines.append ( line )
                line = ''
                
                self.reader_data_available.notify()
                
                self.reader_data_available.release()
                
            else:
                line += data
                
                print 'Line data', len(data)
    
    def writer(self):
        """loop and copy console->serial until EXITCHARCTER character is found"""
        while self.alive:
            self.writer_data_avilable.acquire()
            
            print 'Got lock'
            
            self.writer_data_avilable.wait()
            
            print 'Work available'
            
            for line in self.write_lines:
                print 'WRITE:', line
                self.serial.write(line)
            
            self.write_lines = []
            
            self.serial.flush()
            
            self.writer_data_avilable.release()
    
    def writeline ( self, line ):
        #print 'WRITE:', line
        self.writer_data_avilable.acquire()
        
        self.write_lines.append ( '%s\r\n' % line )
        
        print 'Nofitying writer'
        
        self.writer_data_avilable.notifyAll()
        
        self.writer_data_avilable.release()
        
        print 'Releasing'
        
        #self.serial.write ( '%s\r\n' % line )
        #self.serial.flush()
    
    def readline ( self ):
        self.reader_data_available.acquire()
        
        if len(self.read_lines) == 0:
            self.reader_data_available.wait( 10 )
        
        if len(self.read_lines) == 0:
            self.reader_data_available.release()
            raise Exception('ERROR: No lines read from target')
        
        ret = self.read_lines.pop()
    
        self.reader_data_available.release()
        
        return ret

#port = MyPort(portname, baudrate)
#port.start()

port = serial.Serial(portname, baudrate, timeout = 0.7, dsrdtr = False)
print 'Opened:', port.portstr

time.sleep ( 5 )

def readline ():
#    global port
    return port.readline().rstrip()

def assertLine ( expected_line ):
    global port

    read_line = readline()
    assertEquals ( expected_line, read_line )

def writeline ( line ):
    global port

    port.write ( '%s\r\n' % line )
    #port.writeline ( '%s' % line )

writeline ( 'PROTOCOL' )
assertLine ( '+PROTOCOL 1' )

expected_list = '+LIST bool bool_t char double float integer long short signed_char signed_integer signed_long signed_short unsigned_char unsigned_integer unsigned_long unsigned_short'

print 'Target is running protocol version 1, OK!'

start = datetime.now()
for i in xrange(1, 1001):
    if i % 100 == 0:
        print '%i\r' % i
    
    port.write ( 'LIST /type_tests/properties\r\n' )
    assertLine ( expected_list )

end = datetime.now()
timespan = end - start

used_ms = float(timespan.seconds * 1000 + timespan.microseconds / 1000)

print 'Operation took:', used_ms, 'ms'

total_written = i * len('LIST /type_tests/properties\r\n')
total_read = i * (len(expected_list) + 2)

print 'Total written:', total_written, 'bytes'
print 'Total read:', total_read, 'bytes'

print 'Wrote at', float(total_written) / (used_ms / 1000), 'bytes/s'
print 'Read at', float(total_read) / (used_ms / 1000), 'bytes/s'
print 'Total at', float(total_written + total_read) / (used_ms / 1000), 'bytes/s'
port.close()
