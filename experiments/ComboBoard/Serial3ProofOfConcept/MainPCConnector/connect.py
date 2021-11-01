import time
import socket

# create an INET, STREAMing socket
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# now connect to the server on port 80 - the normal http port
time_start = time.time()
clientsocket.connect(("192.168.2.165", 44444))
counter = 0
message = 'PING' + str(counter)
arr = bytearray(message, 'utf-8')
clientsocket.send(arr)
while counter < 999:
    counter += 1
    clientsocket.recv(16)
    message = 'PING' + str(counter)
    arr = bytearray(message, 'utf-8')
    if counter % 100 == 0:
        print(message)
    clientsocket.send(arr)
time_end = time.time()
print(time_end - time_start)
