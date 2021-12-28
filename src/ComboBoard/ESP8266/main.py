from machine import UART, reset
try:
    import usocket as socket
except ModuleNotFoundError:
    import socket
import gc
gc.collect()


def forward_serial3_to_wifi():
    try:
        msg = cli_sock.recv(64)
    except OSError as e1:
        if str(e1).find("can't read"):
            return
    print("sensor received:", msg)
    uart.write(msg)


def forward_wifi_to_serial3():
    try:
        msg = uart.read(64)
    except OSError as e1:
        if str(e1).find("can't read"):
            return
    print("actuator received:", msg)
    cli_sock.send(msg)


uart = UART(1, 115200)
uart.init(115200, bits=8, parity=None, stop=1)

try:
    srv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv_sock.bind(('', 44444))
except Exception as e:
    print("ERR1:", str(e))
dropout = False
while not dropout:
    try:
        srv_sock.listen(5)
        (cli_sock, address) = srv_sock.accept()
        while True:
            forward_serial3_to_wifi()
            forward_wifi_to_serial3()
    except Exception as e:
        print("ERR2:", str(e))
        dropout = True
    finally:
        cli_sock.close()
