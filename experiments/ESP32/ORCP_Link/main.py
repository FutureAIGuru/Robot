########################################################################
# Setup code goes below, this is called once at start of the program.  #
# boot.py already connected the board to local WiFi.                   #
########################################################################
import time
import sys
import os

import orcp_link as link

try:
  import usocket as socket
except:
  import socket
  
from time import sleep


# create an INET, STREAMing socket
try:
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.bind(('', 44444))
except Exception as e:
	print("ERR:", str(e))

while True:
    try:
        srv.listen(5)
        print('server listening')
        loop = True
        while loop:
            print('waiting for connect request')
            (cs, addr) = srv.accept()
     	    print('request from:', addr)
            print('creating link object')
            orcp = link.ORCP_Link(cs, link._ROBOT)
            print('calling activate')
            loop = orcp.activate()
            
        print('Restart protocol')

    except Exception as e:
        print('EXCEPTION:', e)
        print('Restart server')    
