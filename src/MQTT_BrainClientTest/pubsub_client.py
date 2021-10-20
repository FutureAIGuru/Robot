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
        link_status = 'acquiring'
        return
    print('Brain ignored incoming Startup message')

def handle_hw_config(message):
    link_status = mq.configuring
    print('Brain in CONFIGURE mode', str(message.payload.decode("utf-8")))
    client.publish(mq.swconfig, str(message.payload.decode("utf-8")))
    if str(message.payload.decode("utf-8")) == mq.endconfig:
        link_status = mq.operating
                
def handle_sensor_data(message):
    print('Brain in OPERATING mode', str(message.payload.decode("utf-8")))
    client.publish(mq.actuator, 'ACTUATORSTRING')
                
def on_message(client, userdata, message):
    global link_status
    print('MSG:')
    print('client =', client)
    print('userdata =', userdata)
    if message is not None:
        if str(message.topic) == mq.startup and link_status == mq.unconnected:
            handle_startup(message)
        if str(message.topic) == mq.hwconfig and link_status == mq.configuring:
            handle_hw_config(message)
        if str(message.topic) == mq.sensor and link_status == mq.operating:
            handle_sensor_data(message)

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

while link_status == mq.acquiring:
    time.sleep(1)
    
while link_status == mq.configuring:
    time.sleep(1)
    
while link_status == mq.operating:
    time.sleep(1)
    
# time.sleep(30)
# client.loop_stop()
    