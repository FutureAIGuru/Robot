from umqttsimple import MQTTClient
from machine import UART
import topics
import ubinascii
import machine
import time
import os
import tk

lst  = 'load_files.txt'
vrs  = 'version.txt'

mode = topics.m_u

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

def handle_acquire(msg):
    global mode
    mode = topics.m_c
    print('Brain sent acquire')

def handle_sw_config(msg):
    global mode
    if msg == topics.c_end:
        mode = topics.m_o
    print('Brain sent swconfig')
                
def handle_actuator_data(msg):
    global mode
    print('Brain sent actuator')
    mode = topics.m_c
                
def sub_cb(topic, msg):
    global mode
    if msg is not None:
        print('msg found:', mode, topic, msg)
        if str(topic) == str(topics.t_a):
            handle_acquire(msg)
        if str(topic) == str(topics.t_sc):
            handle_sw_config(msg)
        if str(topic) == str(topics.t_act):
            handle_actuator_data(msg)

def connect_and_subscribe():
    global client_id, mqtt_server
    client = MQTTClient(client_id, mqtt_server)
    client.set_callback(sub_cb)
    client.connect()
    client.subscribe(topics.t_a)
    client.subscribe(topics.t_sc)
    client.subscribe(topics.t_act)
    print('Connected to MQTT broker', mqtt_server)
    return client

def restart_and_reconnect():
    print('Failed to connect to MQTT broker. Reconnecting...')
    time.sleep(1)
    machine.reset()

def publish_configuration():
    msg = topics.c_ir
    print(': ', msg)
    client.publish(topics.t_hc, msg)    
    msg = topics.c_md
    print(': ', msg)
    client.publish(topics.t_hc, msg)    
    msg = topics.c_ws
    print(': ', msg)
    client.publish(topics.t_hc, msg)    
    msg = topics.c_is
    print(': ', msg)
    client.publish(topics.t_hc, msg)    
    msg = topics.c_end
    print(': ', msg)
    client.publish(topics.t_hc, msg)    

def send_sensor_data():
    if UART.any(uart):
        msg = uart.read()
        print('sending sensor data', msg)
        parts = msg.split()
        if parts[0] == 'IrCmd':
            print('sending IR data', msg)
            client.publish(topics.c_ir, msg)    
    # msg = topics.wheelsensor
    # print(': ', msg)
    # client.publish(topics.sensor, msg)    
    # msg = topics.inertialsensor
    # print(': ', msg)
    # client.publish(topics.sensor, msg)    
    pass

check_for_updates()
print('version:', tk.rd_file(vrs, '9.9.9'))

uart = UART(1, 115200)
uart.init(115200, bits=8, parity=None, stop=1)

# Main program after this...

try:
    client = connect_and_subscribe()
except OSError as e:
    restart_and_reconnect()

start_time = time.time()

mode = topics.m_u
while True:
    try:
        client.check_msg()
        if mode == topics.m_u:
            client.publish(topics.t_s, topics.link_id)
            mode = topics.m_a
            print('Startup sent')
        if mode == topics.m_c:
            publish_configuration()
            mode = topics.m_o
        if mode == topics.m_o:
            send_sensor_data()
        if time.time() - start_time > 30:
            restart_and_reconnect()
        else:
            start_time = time.time()
    except OSError as e:
        restart_and_reconnect()
