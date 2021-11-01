########################################################################
# Setup code goes below, this is called once at start of the program.  #
# boot.py already connected the board to local WiFi.                   #
########################################################################
from machine import UART
from time import sleep
import time
import sys
import os

uart = UART(1, 115200)                         # init with given baudrate
uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters

# create an INET, STREAMing socket
while True:
    try:
        # echo serial back onto itself
        avail = UART.any(uart)
        if avail > 0:
            print('reading buffer')
            uart.readinto(read_buff, avail) 
            print('buffer read:', read_buff)
            uart.write(read_buff, avail)
            print('serial write done')
            print(read_buff)
            print('buffer printed')
    except Exception as e:
        print('EXCEPTION', e)
        print('Rebooting...')
        time.sleep(10)
        machine.reset()
