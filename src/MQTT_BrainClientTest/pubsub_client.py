from random import randrange, uniform
import paho.mqtt.client as mqtt 
import time

import tp
mqttBroker ="192.168.2.155" 

mode = tp.m_u
sens_list = []
actr_list = []

def handle_startup(message):
    print('message:', str(message.payload.decode("utf-8")))
    print('identifier:', tp.link_id)
    if str(message.payload.decode("utf-8")) != tp.link_id:
        print('Brain ignored incoming Startup message')
        return
    print('correct robot found')
    client.publish(tp.t_a, str(time.time()))
    mode = tp.m_c
    return

def handle_hw_config(message):
    global mode, sens_list, actr_list
    msg = str(message.payload.decode("utf-8"))
    print('sw_config received', msg)
    if msg[0] == 'S':
        client.publish(tp.t_sc, msg)    
        sens_list.append(msg)
        return
    if msg[0] == 'A':
        client.publish(tp.t_sc, msg)    
        actr_list.append(msg)
        return
    if msg == tp.c_end:
        print('switch to operating mode')
        client.publish(tp.t_sc, msg)    
        mode = tp.m_o
        return
                
def handle_sensor_data(message):
    global sens_list
    msg = str(message.payload.decode("utf-8"))
    parts = msg.split()
    for sens in sens_list:
        if parts[0] == sens.split()[0]:
            sens.recv_payload(parts)
    print('sensor received', msg)
                
def on_message(client, userdata, message):
    global mode
    if message is not None:
        if str(message.topic) == tp.t_s:
            print('handle_startup()')
            handle_startup(message)
            return
        if str(message.topic) == tp.t_hc:
            print('handle_hw_config()')
            handle_hw_config(message)
            return
        if str(message.topic) == tp.t_sns:
            print('handle_sensor_data()')
            handle_sensor_data(message)
            return

def send_actuators():
    global client, actr_list, mode
    if mode == tp.m_o:
        for actr in actr_list:
            client.publish(tp.t_act, actr)
        
# Main program after this...

client = mqtt.Client("BrainSimII")
client.connect(mqttBroker) 
client.loop_start()
client.subscribe(tp.t_s)
client.subscribe(tp.t_hc) 
client.subscribe(tp.t_sns)
client.on_message=on_message 

# robot leads the conversation until 
# operating mode is established...
mode = tp.m_u
while mode == tp.m_u:
    time.sleep(0.1)
while mode == tp.m_a:
    time.sleep(0.1)
while mode == tp.m_c:
    time.sleep(0.1)
    
# when in operating mode, we send the 
# actuators from this loop...
print('started in mode:', mode)
while mode == tp.m_o:
    send_actuators()
    time.sleep(1)
