import time
from umqttsimple import MQTTClient
from machine import UART
import ubinascii
import machine
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
        print('RECEIVED:', topic, cfg.unpack_drive(msg))
    if topic == cfg.q_wheels:
        print('RECEIVED:', topic, cfg.unpack_wheels(msg))

def connect_and_subscribe():
    global client_id, mqtt_server
    client = MQTTClient(client_id, mqtt_server)
    client.set_callback(sub_cb)
    client.connect()
    client.subscribe(cfg.q_drive)
    print('Connected to %s MQTT broker, subscribed to %s topic' % (mqtt_server, cfg.q_drive))
    return client

def restart_and_reconnect():
    print('Failed to connect to MQTT broker. Reconnecting...')
    time.sleep(5)
    machine.reset()

def receive_and_process(uart):
    rcvd = client.check_msg()
    if rcvd != None: 
        print('RECEIVED:', rcvd)
        uart.write(rvcd)
    return rcvd

def prepare_and_publish(uart):
    global last_msg, msg_interval, counter
    if (time.time() - last_msg) > msg_interval:
        msg = cfg.pack_wheels(125, 245, 345, 755)
        client.publish(cfg.q_wheels, msg)
        print('PUBLISHED:', cfg.q_wheels, cfg.unpack_wheels(msg))
        last_msg = time.time()
        counter += 1

"""Executing code below."""
uart = UART(1, 115200)
uart.init(115200, bits=8, parity=None, stop=1)
try:
    client = connect_and_subscribe()
except OSError as e:
    restart_and_reconnect()

while True:
    try:
        rcvd = receive_and_process(uart)
        sent = prepare_and_publish(uart)
    except OSError as e:
        restart_and_reconnect()
