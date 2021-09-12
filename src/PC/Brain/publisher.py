import paho.mqtt.client as mqtt
import time
import mqtt_config as cfg

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")

# The callback function for received message
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

client = mqtt.Client(client_id="RyZEN_Publisher")
client.on_connect = on_connect
client.on_message = on_message
client.connect("192.168.2.155", 1883, 60)
print('connected to 192.168.2.155:1883')
i = 0
while True:
    i += 1
    load = time.time()
    client.publish(cfg.q_drive, payload=load, qos=0, retain=False)
    print('PAYLOAD:', load)
    # client.publish(cfg.q_wheels, payload=i, qos=0, retain=False)
    time.sleep(2)
       
