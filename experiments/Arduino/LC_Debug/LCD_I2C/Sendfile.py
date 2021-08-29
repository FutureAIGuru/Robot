import time
import serial

def test():
    print("sent")
    ardu= serial.Serial('COM8', 9600, timeout=.1)
    time.sleep(1)
    # with open('testdemo.txt') as f:
    #     read_data = f.read()
    #     for line in read_data:
    ardu.write('Testing the Serial'.encode());
    time.sleep(1)
    ardu.close()

test()            
       