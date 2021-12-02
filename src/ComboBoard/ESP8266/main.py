from umqttsimple import MQTTClient
from machine import UART
import ubinascii
import machine
import time
import os

import tp   # MQTT topics
import tk   # toolkit

lst  = 'load_files.txt'
vrs  = 'version.txt'

uart = UART(1, 115200)
uart.init(115200, bits=8, parity=None, stop=1)

# Uncomment this to turn the Serial echo on for SerialTest
# buff = '0'
# while True:
#     if uart.any() > 0:
#         buff = uart.read(1)
#         uart.write(buff)

mode = tp.m_u
sens_list = [
    'S0 x2 p0 m1 e1 t100 T200',
    'S1 x2 p0 m1 e1 t100 T200',
    'S2 x2 p0 m1 e1 t100 T200',
    'S3 x2 p0 m1 e1 t100 T200'
    ]
actr_list = [
    'A0 x0 p022223 e1 T90 c0',
    'A1 x0 p032425 e1 T90 c0',
    'A2 x0 p042627 e1 T90 c0',
    'A3 x0 p052829 e1 T90 c0'    
    ]

def upd(src):
    tk.del_file(src)
    txt = tk.do_req(src)
    tk.wr_file(src, txt)

def check_for_updates():
    old_vrs = tk.rd_file(vrs, '0.0.0')
    new_vrs = tk.do_req(vrs, '0.0.0')
    if str(old_vrs) == str(new_vrs):
        print('VERSIONS EQUAL, NO UPDATE')
        return
    tk.wr_file(vrs, new_vrs)
    txt = tk.do_req(lst)
    lines = txt.split('\n')
    for line in lines:
        if line.endswith('\r'):
            line = line[0:-1]
        if len(line) > 0:
            upd(line)
    machine.reset()

def connect_and_subscribe():
    global client_id, mqtt_server
    client = MQTTClient(client_id, mqtt_server)
    client.set_callback(on_message)
    client.connect()
    client.subscribe(tp.t_a)
    client.subscribe(tp.t_sc)
    client.subscribe(tp.t_act)
    print('Connected to MQTT broker', mqtt_server)
    return client

def restart_and_reconnect():
    print('Failed to connect to MQTT broker. Reconnecting...')
    time.sleep(10)
    machine.reset()

def handle_acquire(msg):
    """Handle a received acquire message."""
    global mode
    mode = tp.m_c
    print('connection acquired')

def handle_sw_config(msg):
    """Handle incoming sw_config messages."""
    global mode, sens_list, actr_list
    msg = str(msg)
    print('sw_config received', msg)
        
    if msg == str(tp.c_end):
        print('switch to operating mode')
        mode = tp.m_o
        return
    if msg in sens_list:
        client.publish(tp.t_hc, msg)    
        return
    if msg in actr_list:
        client.subscribe(msg)
        return
    if msg == tp.c_end:
        mode = tp.m_o
                
def handle_actuator_data(msg):
    """Handler for actuator messages from the brain."""
    global actr_list
    msg_parts = msg.split()
    for actr in actr_list:
        actr_parts = actr.split()
        if actr_parts[0] == msg_parts[0]:
            print('actuator received:', msg)
                
def send_sensor_data():
    """Send sensor messages for all sensors."""
    global client, sens_list
    for sens in sens_list:
        client.publish(tp.t_sns, sens)

def on_message(topic, msg):
    """Callback routine for MQTT client."""
    global mode, sens_list, actr_list
    if msg is not None:
        # print('msg found:', mode, topic, msg)
        if str(topic) == str(tp.t_a):
            mode = tp.m_a
            handle_acquire(msg)
            return
        if str(topic) == str(tp.t_sc) and mode == tp.m_c:
            handle_sw_config(msg)
            time.sleep(10)
            return
        if mode == tp.m_o:
            handle_actuator_data(msg)
            return

def send_startup_message():
    """Sending the startup invitation."""
    global client, mode
    # print('Sending startup...')
    client.publish(tp.t_s, tp.link_id)
    mode = tp.m_a

def send_hw_configuration():
    """Send hw_config messages to the brain."""
    global sens_list, actr_list
    for conf in sens_list:
        print('publishing sensor:', conf)
        client.publish(tp.t_hc, conf)    
    for conf in actr_list:
        print('subscribing to actuator:', conf.split()[0])
        client.publish(tp.t_hc, conf)    
        client.subscribe(conf.split()[0])
    client.publish(tp.t_hc, tp.c_end)

def recv_sensor_data():
    try:
        global sens_list, uart, client
        sensor = uart.readline()
        if sensor[0] != 'S':
            print('unrecognized sensor input')
            return 
        sensor_parts = sensor.split()
        for sens in sens_list:
            if sens.split()[0] == sensor.split()[0]:
                client.publish(tp.t_sns, sensor)
    except OSError as e:
        # print('serial port exception', e)
        return
    
# commented out because it slows development.
# tk.check_for_updates()
# print('version:', tk.rd_file(vrs, '9.9.9'))

# Main program after this...

try:
    client = connect_and_subscribe()
except OSError as e:
    print('connect_and_subscribe exception', e)
    time.sleep(5)
    # restart_and_reconnect()

start_time = time.time()

# robot leads the conversation until 
# operating mode is established...
mode = tp.m_u
hw_sent = False
while True:
    try:
        # make sure we handled incoming sensor messages
        recv_sensor_data()
        # check for incoming first...
        client.check_msg()
        if mode == tp.m_u:
            send_startup_message()
        if mode == tp.m_c and not hw_sent:
            send_hw_configuration()
            hw_sent = True
        if mode == tp.m_o:   
            send_sensor_data()
            time.sleep(1)
        if time.time() - start_time > 30:
            restart_and_reconnect()
        else:
            start_time = time.time()
    except OSError as e:
        print('main code exception', e)
        time.sleep(5)
        restart_and_reconnect()
