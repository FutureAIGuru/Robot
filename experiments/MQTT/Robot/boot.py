import time
from umqttsimple import MQTTClient
import ubinascii
import machine
import micropython
import network
import esp
esp.osdebug(None)
import gc
gc.collect()

ssid = 'NewReality4Me'
password = 'S@ferThanB4!'
mqtt_server = '192.168.2.155'

client_id = 'Robot_20210905'   # ubinascii.hexlify(machine.unique_id())
topic_sub = b'hello'
topic_pub = b'notification'

last_message = 0
message_interval = 5
counter = 0

station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  pass

print('Connection successful')
print(station.ifconfig())