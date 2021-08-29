try:
  import usocket as socket
except:
  import socket
  
from time import sleep
from machine import Pin
 
import network
import esp
esp.osdebug(None)
 
import gc
gc.collect()
 
ssid = 'NewReality4Me'
password = 'S@ferThanB4!'
 
print('Attempting Connection to WiFi', ssid)
station = network.WLAN(network.STA_IF)
 
station.active(True)
station.connect(ssid, password)
 
while station.isconnected() == False:
  pass
 
print('Connection successful')
print(station.ifconfig())
