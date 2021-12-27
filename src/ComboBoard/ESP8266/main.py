from machine import UART, reset
try:
    import usocket as socket
except Exception:
    import socket
import gc
gc.collect()


def broadcast_identity():
    print("Create UDP socket")
    cli_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    print("set broadcast parameters")
    cli_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    cli_sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    print("send broadcast message")
    cli_sock.sendto('Robot_20211224', ('255.255.255.255', 3333))


def forward_serial3_to_wifi():
    try:
        msg = cli_sock.recv(64)
    except OSError as e1:
        if str(e1).find("can't read"):
            return
    uart.write(msg)


def forward_wifi_to_serial3():
    try:
        msg = uart.read(64)
    except OSError as e1:
        if str(e1).find("can't read"):
            return
    cli_sock.send(msg)


uart = UART(1, 115200)
uart.init(115200, bits=8, parity=None, stop=1)

try:
    broadcast_identity()

    # now go into access point configuration
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
        if cli_sock is not None:
            cli_sock.close()
