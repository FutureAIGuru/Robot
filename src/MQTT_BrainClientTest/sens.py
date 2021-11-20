
class Sensor:

    def __init__(self, label):
        self.label = label
        
    def receive_payload(self, parts):
        if parts[0] == tp.s_ir:
            return self._set_payload_infraredsensor(parts[1:])
        if parts[0] == tp.s_ws:
            return self._set_payload_wheelsensor(parts[1:])
        if parts[0] == tp.s_is:
            return self._set_payload_inertialsensor(parts[1:])
            
    def send_payload(self):
        if self.label == tp.s_ir:
            return self._payload_infraredsensor()
        if self.label == tp.s_ws:
            return self._payload_wheelsensor()
        if self.label == tp.s_is:
            return self._payload_inertialsensor()
            
    def _set_payload_infraredsensor(self, command):
        tp._pl_arm_command = command
        print('INFRAREDSENSOR:', self._payload_infraredsensor())
    
    def _payload_infraredsensor(self):
        return  tp._pl_arm_infraredsensor

    def _set_payload_wheelsensors(self, frontleft, frontright, rearleft, rearright):
        tp._pl_ws_frontleft = frontleft
        tp._pl_ws_frontright = frontright
        tp._pl_ws_rearleft = rearleft
        tp._pl_ws_rearright = rearright
        print('WHEELSENSORS:', self._payload_wheelsensors())
    
    def _payload_wheelsensors(self):
        return  tp._pl_ws_frontleft + ' ' + \
                tp._pl_ws_frontright + ' ' + \
                tp._pl_ws_rearleft + ' ' + \
                tp._pl_ws_rearright
                
    def _set_payload_inertialsensor(self, x, y, z):
        tp._pl_is_x = x
        tp._pl_is_y = y
        tp._pl_is_z = z
        print('INERTIALSENSOR:', self._payload_inertialsensor())
    
    def _payload_inertialsensor(self):
        return  tp._pl_is_x + ' ' + \
                tp._pl_is_y + ' ' + \
                tp._pl_is_z

                

