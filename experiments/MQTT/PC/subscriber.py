import paho.mqtt.client as mqtt

# The callback function of connection
def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe("a/#")
    client.subscribe("hello")

# The callback function for received message
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

client = mqtt.Client(client_id="RyZEN_Subscriber")
client.on_connect = on_connect
client.on_message = on_message
client.connect("192.168.2.155", 1883, 60)
print('connected to 192.168.2.155:1883')
client.loop_forever()