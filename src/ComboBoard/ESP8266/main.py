from umqttsimple import MQTTClient
import topics
import ubinascii
import machine
import time
import os
import tk

lst  = 'load_files.txt'
vrs  = 'version.txt'

status = topics.unconnected

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

def handle_acquire(message):
    global status
    status = topics.configuring
    print('Brain sent acquire message')

def handle_sw_config(message):
    global status
    if msg == topics.endconfig:
        status = topics.operating
    print('Brain sent swconfig message')
                
def handle_actuator_data(message):
    global status
    print('Brain sent actuator message')
                
def sub_cb(topic, msg):
    global status
    print((topic, msg))
    if topic == topics.acquire:
        print('Robot received ' + str(topics.acquire) + ' message.')
        status = topics.acquiring
    if msg is not None:
        if str(topic) == str(topics.acquire) and status == topics.unconnected:
            handle_acquire(msg)
        if str(topic) == str(topics.swconfig) and status == topics.configuring:
            handle_sw_config(msg)
        if str(topic) == str(topics.actuator) and status == topics.operating:
            handle_actuator_data(msg)

def connect_and_subscribe():
    global client_id, mqtt_server
    client = MQTTClient(client_id, mqtt_server)
    client.set_callback(sub_cb)
    client.connect()
    client.subscribe(topics.acquire)
    client.subscribe(topics.swconfig)
    client.subscribe(topics.actuator)
    print('Connected to %s MQTT broker, subscribed to %s topic' % (mqtt_server, topics.swconfig))
    return client

def restart_and_reconnect():
    print('Failed to connect to MQTT broker. Reconnecting...')
    time.sleep(1)
    machine.reset()

def publish_configuration():
    msg = topics.motordrive
    print(': ', msg)
    client.publish(topics.hwconfig, msg)    
    msg = topics.wheelsensor
    print(': ', msg)
    client.publish(topics.hwconfig, msg)    
    msg = topics.inertialsensor
    print(': ', msg)
    client.publish(topics.hwconfig, msg)    
    msg = topics.endconfig
    print(': ', msg)
    client.publish(topics.hwconfig, msg)    

def send_sensor_data():
    msg = topics.wheelsensor
    print(': ', msg)
    client.publish(topics.sensor, msg)    
    msg = topics.inertialsensor
    print(': ', msg)
    client.publish(topics.sensor, msg)    

check_for_updates()
print('version:', tk.rd_file(vrs, '9.9.9'))

# Main program after this...
try:
    client = connect_and_subscribe()
except OSError as e:
    restart_and_reconnect()

start_time = time.time()
client.publish(topics.startup, topics.link_identifier)
print('Sending topics.startup', topics.link_identifier)

while True:
    try:
        client.check_msg()
        if status == topics.acquiring:
            publish_configuration()
            status = topics.operating
        if time.time() - start_time > 30:
            restart_and_reconnect()
        else:
            start_time = time.time()
        if status == topics.operating:
            send_sensor_data()
    except OSError as e:
        restart_and_reconnect()
