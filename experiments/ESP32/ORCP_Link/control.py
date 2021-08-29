import time
import socket
import orcp_link as link

max_rounds = 0
while max_rounds < 10:
    cs = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    cs.connect(("192.168.2.162", 44444))
    try:
        orcp = link.ORCP_Link(cs, link._BRAIN)
        loop = orcp.activate()
        
    except Exception as e:
        print("ERR:", str(e))
    finally:
        cs.close()
        max_rounds += 1
           

