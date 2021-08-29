import time
import serial

# configure the serial connections
ser = serial.Serial(port='COM8', baudrate=9600)

ser.isOpen()

print('Enter your messages below.\r\nInsert "exit()" to leave the application.')

line=''
while 1 :
    # get keyboard input
    line = input(">> ")
    if line == 'exit()':
        ser.close()
        exit()
    else:
        export = bytearray(line, 'UTF-8')
        ser.write(export)
        
        out = ''
        # let's wait one second before reading output
        time.sleep(1)
        while ser.inWaiting() > 0:
            out += ser.read(1)

        if out != '':
            print(">>" + out)