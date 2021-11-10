#!/usr/bin/python
import serial
import sys
import time
from PIL import Image
import numpy

def mapColorToChar(avg):
    if(avg <= 16*1):
        return 'f'
    elif(avg <= 16*2):
        return 'e'
    elif(avg <= 16*3):
        return 'd'
    elif(avg <= 16*4):
        return 'c'
    elif(avg <= 16*5):
        return 'b'
    elif(avg <= 16*6):
        return 'a'
    elif(avg <= 16*7):
        return '9'
    elif(avg <= 16*8):
        return '8'
    elif(avg <= 16*9):
        return '7'
    elif(avg <= 16*10):
        return '6'
    elif(avg <= 16*11):
        return '5'
    elif(avg <= 16*12):
        return '4'
    elif(avg <= 16*13):
        return '3'
    elif(avg <= 16*14):
        return '2'
    elif(avg <= 16*15):
        return '1'
    else:
        return '0'

if(len(sys.argv) < 3):
    print("Usage: " + sys.argv[0] + " [tty] [filename]");
    exit(1);
ttyname = sys.argv[1]
filename = sys.argv[2]
lines = []
with Image.open(filename) as f:
    data = numpy.array(f)
    for row in data:
        outrow = ""
        for col in row:
            if(not isinstance(col, numpy.uint8)):
                sumpixs = 0
                numpixs = 0
                for pix in col:
                    sumpixs += pix
                    numpixs += 1
                avg = sumpixs/numpixs
                outcol = mapColorToChar(avg);
                outrow += outcol
            else:
                outcol = mapColorToChar(col)
                outrow += outcol
        if(len(outrow) != 128):
            print("ERROR! wrong width of image(" + str(len(outrow)) + ")!")
            exit(1);
        outrow += '\n'
        lines.append(outrow)

with serial.Serial(ttyname, 115200, timeout=5) as s:
    line = s.readline().decode('ascii')
    if(line != "Init\r\n"):
        print("No init message(" + line + ")!");
    
            
    
    
    for line in lines:
        #print(line)
        s.write(line.encode('ascii'));
        print(str(s.readline()))
        print(str(s.readline()))
