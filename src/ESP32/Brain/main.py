import time
from umqttsimple import MQTTClient
import ubinascii
import machine
from machine import UART
import micropython
import network
import esp
esp.osdebug(None)
import gc
gc.collect()
import mqtt_config as cfg

last_msg = 0
msg_interval = 5
counter = 0

def sub_cb(topic, msg):
    if topic == cfg.q_drive:
        print(topic, cfg.unpack_drive(msg))
    if topic == cfg.q_wheels:
        print(topic, cfg.unpack_wheels(msg))

def connect_and_subscribe():
  global client_id, mqtt_server
  client = MQTTClient(client_id, mqtt_server)
  client.set_callback(sub_cb)
  client.connect()
  client.subscribe(cfg.q_wheels)
  print('Connected to %s MQTT broker, subscribed to %s topic' % (mqtt_server, cfg.q_wheels))
  return client

def restart_and_reconnect():
  print('Failed to connect to MQTT broker. Reconnecting...')
  time.sleep(10)
  machine.reset()

def receive_and_process():
    rcvd = client.check_msg()
    if rcvd != None: print('RECEIVED:', rcvd)
    return rcvd

def prepare_and_publish():
    global last_msg, msg_interval
    if (time.time() - last_msg) > msg_interval:
        msg = cfg.pack_drive('Forward', 'TurnRight', 127)
        client.publish(cfg.q_drive, msg)
        print('PUBLISHED:', cfg.q_drive, msg)
        last_msg = time.time()

"""Executing code below."""
uart = UART(1, 115200)
uart.init(115200, bits=8, parity=None, stop=1)
try:
    client = connect_and_subscribe()
except OSError as e:
    restart_and_reconnect()

while True:
    try:
        rcvd = receive_and_process()
        sent = prepare_and_publish()
    except OSError as e:
        restart_and_reconnect()
