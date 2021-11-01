import paho.mqtt.client as mqtt 
import robot_mqtt_topics as mq
from random import randrange, uniform
import time

mqttBroker ="192.168.2.155" 

link_status = mq.unconnected

def handle_startup(message):
    print('message:', str(message.payload.decode("utf-8")))
    print('identifier:', mq.link_identifier)
    if str(message.payload.decode("utf-8")) == mq.link_identifier:
        print('correct robot found')
        print('Brain went into ACQUIRE mode', str(message.payload.decode("utf-8")))
        client.publish(mq.acquire, str(time.time()))
        link_status = mq.configuring
        return
    print('Brain ignored incoming Startup message')

def handle_hw_config(message):
    link_status = mq.configuring
    msg = str(message.payload.decode("utf-8"))
    print('Brain in CONFIGURE mode', msg)
    client.publish(mq.swconfig, msg)
    if msg == mq.endconfig:
        link_status = mq.operating
                
def handle_sensor_data(message):
    msg = str(message.payload.decode("utf-8"))
    parts = msg.split()
    print('Brain in OPERATING mode', msg)
    if parts[0] == 'IrCmd':
        print('Infrared command received:' , msg)    
                
def on_message(client, userdata, message):
    global link_status
    if message is not None:
        if str(message.topic) == mq.startup:
            print('handle_startup()')
            handle_startup(message)
        if str(message.topic) == mq.hwconfig:
            print('handle_hw_config()')
            handle_hw_config(message)
        if str(message.topic) == mq.sensor:
            print('handle_sensor_data()')
            handle_sensor_data(message)

# Main program after this...

client = mqtt.Client("Brain_Publisher")
client.connect(mqttBroker) 
client.loop_start()
client.subscribe(mq.startup)
client.subscribe(mq.hwconfig)
client.subscribe(mq.sensor)
client.on_message=on_message 

link_status = mq.unconnected
while link_status == mq.unconnected:
    time.sleep(1)
link_status = mq.acquiring
while link_status == mq.acquiring:
    time.sleep(1)
link_status = mq.configuring
while link_status == mq.configuring:
    time.sleep(1)
link_status = mq.operating
while link_status == mq.operating:
    time.sleep(1)
    
# time.sleep(30)
# client.loop_stop()
    