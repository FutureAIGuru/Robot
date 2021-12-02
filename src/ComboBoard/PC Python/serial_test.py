from datetime import datetime
import serial
import random
import time

now = datetime.now()
while True:
    with serial.Serial('COM12', 115200, timeout=.00000001) as ser:  
        ser.write(b'0')
        echo = ser.read()
        for i in range(500):
            new_char = random.choice([b'0', b'1', b'2', b'3', b'4', b'5', b'6', b'7',b'8', b'9'])
            ser.write(new_char)
            echo = ser.read()
            # print(echo)
            # assert new_char == echo
        ser.write(b'1')
        echo = ser.read()
    old_now = now
    now = datetime.now()
    print(now - old_now)
