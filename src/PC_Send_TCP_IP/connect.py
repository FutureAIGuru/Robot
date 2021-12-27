import time
from socket import *


def send_message(msg, sock):
    print("sending message:", msg)
    arr = bytearray(msg, 'utf-8')
    sock.send(arr)

print("creating UDP socket")
s=socket(AF_INET, SOCK_DGRAM)
print("binding socket to port 3333")
s.bind(('', 3333))
print("calling recvfrom")
m = s.recvfrom(1024)
print(m)

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("socket created")
clientsocket.connect(("192.168.2.165", 44444))
print("WiFi connected")
counter = 0
while True:
    counter += 1
    send_message("A0 FL x0 c0 p022223 T180 e1", clientsocket)
    send_message("A1 FR x0 c0 p032425 T180 e1", clientsocket)
    send_message("A2 RL x0 c0 p042627 T180 e1", clientsocket)
    send_message("A3 RR x0 c0 p052829 T180 e1", clientsocket)
    time.sleep(1)
