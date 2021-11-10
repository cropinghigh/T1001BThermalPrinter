#!/usr/bin/python
import serial
import sys
import time

if(len(sys.argv) < 3):
    print("Usage: " + sys.argv[0] + " [tty] [filename]");
    exit(0);
ttyname = sys.argv[1]
filename = sys.argv[2]
with open(filename) as f:
    with serial.Serial(ttyname, 115200, timeout=5) as s:
        line = s.readline().decode('ascii')
        if(line != "Init\r\n"):
            print("No init message(" + line + ")!");
        lines = f.readlines()
        for line in lines:
            s.write(line.encode('ascii'));
            print(str(s.readline()))
            print(str(s.readline()))
