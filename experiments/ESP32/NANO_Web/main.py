########################################################################
# Setup code goes below, this is called once at start of the program  #
########################################################################
import machine
import time
import esp

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 80))
s.listen(5)

rtc = machine.RTC()
rtc.datetime((2021,08,22,19,0,0,0,0))

########################################################################
# The various pages returned to the webclient are methods defined here #
########################################################################

class webResponse():

    def webroot(self):
        return self._read_file('pages/index.html')
        
    def webtest(self):
        return self._read_file('pages/test.html')
        
    def dynamic1(self):
        dynamic = self._read_file('pages/dynamic1.html')
        curr_time = time.ticks_ms()
        return dynamic.replace('{{{TIME}}}', str(curr_time))
        
    def dynamic2(self):
        dynamic = self._read_file('pages/dynamic2.html')
        curr_time = rtc.datetime()
        return dynamic.replace('{{{TIME}}}', str(curr_time))
        
    def showsvg(self):
        return self._read_file('images/test-icon.svg')
        
    def error_404(self):
        return self._read_file('pages/404.html')
        
    def _read_file(self, file_name):
        with open(file_name, 'r') as file:
            return file.read()
        
    
class webRequest():

    def __init__(self, request_string, responses):
        self._request_parts = request_string[2:-1].split('\\r\\n')
        self._responses = responses
        
    def request_action(self):
        return self._request_parts[0][0:-9]
        
    def response(self):
        return  str(webRoute(self, self._responses).get_route())


class webRoute():

    def __init__(self, request, responses):
        self._request = request
        self._responses = responses
        
    def get_route(self):
        route = None
        try:
            route = routing_table[self._request.request_action()]
        except:
            route = responses.error_404()
        finally:
            return route


while True:
    ###################################################################
    # Loop code goes inside the loop here, this is called repeatedly: #
    ###################################################################
    try:
        if gc.mem_free() < 102000:
            gc.collect()
        conn, addr = s.accept()
        conn.settimeout(3.0)
        print('Got a connection from %s' % str(addr))
        request = conn.recv(1024)
        conn.settimeout(None)
        responses = webResponse()   # create response object for the routing table
        routing_table =                               \
        {                                             \
            "GET /":            responses.webroot(),  \
            "GET /test":        responses.webtest(),  \
            "GET /dynamic1":    responses.dynamic1(), \
            "GET /dynamic2":    responses.dynamic2(), \
            "GET /showsvg":     responses.showsvg()   \
        }
        request = webRequest(str(request), responses)
        conn.send('HTTP/1.1 200 OK\n')
        conn.send('Content-Type: text/html\n')
        conn.send('Connection: close\n\n')
        conn.sendall(request.response())
        conn.close()
    except OSError as e:
        conn.close()
        print('Connection closed')
