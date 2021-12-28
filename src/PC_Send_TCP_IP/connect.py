import time
import socket

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
time_start = time.time()
clientsocket.connect(("192.168.2.165", 44444))
counter = 0
while True:
    counter += 1
    message = 'SEND' + str(counter % 2)
    arr = bytearray(message, 'utf-8')
    clientsocket.send(arr)
    time.sleep(1)
