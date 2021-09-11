from umqttsimple import MQTTClient
import micropython
import network
import machine
import time
import os

import esp
esp.osdebug(None)
import gc
gc.collect()

def error(msg):
    print('ERROR:', msg)
    sleep(5)

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
print('disconnect pin 12 from GND to continue')
while pin.value() == 0: 
    time.sleep(1)

topic_sub = b'hello'
topic_pub = b'notification'

last_message = 0
message_interval = 5
counter = 0

station = network.WLAN(network.STA_IF)
if not station.isconnected():
    print('Trying to connect to', ssid)
    station.active(True)
    station.connect(ssid, password)
    while not station.isconnected():
        pass
    print('Connection successful')
    print(station.ifconfig())
