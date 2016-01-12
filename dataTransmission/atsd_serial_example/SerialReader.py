#!/usr/bin/python3
import serial
import socket

# config
ser = serial.Serial(
    port='/dev/ttyACM0', #change to your port
    baudrate=9600,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS
)

ser.isOpen()
conn = socket.socket()
conn.connect( ("atsd_server", 9081) ) #change to your atsd servername and actual port

char = ''
out = ''
while 1 :
    while ser.inWaiting() > 0:
        char = ser.read(1).decode()
        if char != '\n':
            out += char 
        else:
            command = "series e:arduino-1 m:temperature="+out
            conn.send((command + "\n").encode())
            print(command)
            out = ''

conn.close()
