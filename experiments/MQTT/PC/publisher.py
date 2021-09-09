import paho.mqtt.client as mqtt
import time

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
    client.publish('a/b', payload=time.time(), qos=0, retain=False)
    client.publish('notification', payload=i, qos=0, retain=False)
    time.sleep(2)
       
