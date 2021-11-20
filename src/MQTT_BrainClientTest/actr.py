import tp

class Actuator:

    def __init__(self, label):
        self.label = label
        
    def receive_payload(self, parts):
        if parts[0] == tp.a_arm:
            return self._set_payload_arm(parts[1:])
        if parts[0] == tp.a_wrist:
            return self._set_payload_wrist(parts[1:])
        if parts[0] == tp.a_hand:
            return self._set_payload_hand(parts[1:])
        if parts[0] == tp.a_cam:
            return self._set_payload_cam(parts[1:])
        if parts[0] == tp.a_md:
            return self._set_payload_md(parts[1:])
            
    def send_payload(self):
        if self.label == tp.a_arm:
            return self._payload_arm()
        if self.label == tp.a_wrist:
            return self._payload_wrist()
        if self.label == tp.a_hand:
            return self._payload_hand()
        if self.label == tp.a_cam:
            return self._payload_cam()
        if self.label == tp.a_md:
            return self._payload_md()
            
    def _set_payload_arm(self, rotate, reach, elbow, wrist):
        tp._pl_arm_rotate = rotate
        tp._pl_arm_reach = reach
        tp._pl_arm_elbow = elbow
        tp._pl_arm_wrist = wrist
    
    def _payload_arm(self):
        return  tp._pl_arm_rotate + ' ' + \
                tp._pl_arm_reach + ' ' + \
                tp._pl_arm_elbow + ' ' + \
                tp._pl_arm_wrist

    def _set_payload_wrist(self, rotate):
        tp._pl_wrist_rotate = rotate
    
    def _payload_wrist(self):
        return tp._pl_wrist_rotate

    def _set_payload_hand(self, grip):
        tp._pl_hand_grip = grip
    
    def _payload_hand(self):
        return tp._pl_hand_grip

    def _set_payload_cam(self, pan, tilt):
        tp._pl_cam_pan = pan
        tp._pl_cam_tilt = tilt
    
    def _payload_cam(self):
        return tp._pl_cam_pan + ' ' + \
               tp._pl_cam_tilt

    def _set_payload_md(self, direction, turn, speed):
        tp._pl_md_direction = direction
        tp._pl_md_turn = turn
        tp._pl_md_speed = speed
    
    def _payload_md(self):
        return tp._pl_md_direction + ' ' + \
               tp._pl_md_turn + ' ' + \
               tp._pl_md_speed

