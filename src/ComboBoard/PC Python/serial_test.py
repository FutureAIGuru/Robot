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
            new_char = random.choice([b'1', b'0'])
            ser.write(new_char)
            echo = ser.read()
            # print(echo)
            # assert new_char == echo
        ser.write(b'1')
        echo = ser.read()
    old_now = now
    now = datetime.now()
    print(now - old_now)
