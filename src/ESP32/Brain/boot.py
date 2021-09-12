import time
from umqttsimple import MQTTClient
import ubinascii
import machine
import micropython
import network
import os
import esp
esp.osdebug(None)
import gc
gc.collect()

def acquire(name, force):
    f_val= ''
    f_name = name + '.cfg'
    if f_name in os.listdir():
        try:
            f = open(f_name, 'r')
            f_val= f.readline()
            f.close()
        except Exception:
            print(name, 'not configured yet')
    reply = 'no'
    if force == 0: 
        f_val = ''
    while f_val== '':
        prompt = 'Enter value for ' + name + ':'
        f_val= input(prompt)
        prompt = 'Is value ' + f_val+ ' correct? (Y/n):'
        reply = input(prompt)
        if reply != 'y' and reply != '':
            f_val= ''
    f = open(f_name, 'w')
    f.write(f_val)
    f.close()
    return f_val
    
pin = machine.Pin(12, machine.Pin.IN, machine.Pin.PULL_UP)
replace = pin.value()
ssid = acquire('SSID', replace)
password = acquire('password', replace)
mqtt_server = acquire('mqtt_server', replace)
client_id = acquire('client_id', replace)
if pin(12) == 0:
    print('disconnect pin 12 from GND to continue')
while pin.value() == 0: 
    time.sleep(1)

station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  pass

print('Connection successful')
print(station.ifconfig())
