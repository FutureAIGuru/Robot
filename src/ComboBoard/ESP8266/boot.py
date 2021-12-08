import time
import machine
import micropython
import network
import esp
esp.osdebug(None)
import os
import gc
gc.collect()

def acquire(name):
    acq = ''
    f_name = name + '.cfg'
    if f_name in os.listdir(''):
        try:
            f = open(f_name, 'r')
            acq = f.readline()
            f.close()
        except Exception:
            print(name, 'not configured yet')
            pass
    reply = 'no'
    while acq == '':
        prompt = 'Enter value for ' + name + ':'
        acq = input(prompt)
        prompt = 'Is value ' + acq + ' correct? (Y/n):'
        reply = input(prompt)
        if reply != 'y':
            acq = ''
    f = open(f_name, 'w')
    f.write(acq)
    f.close()
    return acq
    
# set up the wifi client connection
ssid = acquire('SSID')
password = acquire('password')
mqtt_server = acquire('mqtt_server')
client_id = acquire('client_id')

last_message = 0
message_interval = 5
counter = 0

station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

retries = 10
while station.isconnected() == False and retries > 0:
    retries -= 1
    time.sleep(10)

if retries == 0 or station.isconnected() == False:
    # os.remove('SSID.cfg')
    # os.remove('password.cfg')
    machine.reset()

print('Connection successful')
print(station.ifconfig())