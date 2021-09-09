########################################################################
# Open Robot Communications Protocol                                   #
# Programmed by Andre Slabber                                          #
# License is MIT license                                               #
########################################################################
import time
import sys
import os

try:
  import usocket as socket
except:
  import socket
  
_MSG_LEN  = 4
_TYPE_LEN = 7
_DGT_4LEN = 4
_DGT_6LEN = 6

_BRAIN = True
_ROBOT = False

_ERR = True
_OK  = False

_ERR_PRTCL = 'ERROR|PROTOCOL|'

_MSG_START = 'START|ORCP|20210823202020|'
_MSG_INIT  = 'INIT_|ORCP|20210823202020|'
_MSG_HWCFG = 'HWCFG|'
_MSG_SWCFG = 'SWCFG|'
_MSG_SENSE = 'SENSE|'
_MSG_ACTNW = 'ACTNW|'
_MSG_NOOP  = 'NOOP_|'
_MSG_CFG_0 = '0000|'

class TimeOut():

    def __init__(self, max_time):
        self._ticks = self.now()
        self._timeout = max_time
        
    def reset(self):
        self._ticks = self.now()
        
    def timeout(self):
        return self._timeout
        
    def now(self):
        ticks = 0
        if sys.implementation.name == "micropython":
            ticks = time.ticks_ms()
        else:
            ticks = round(time.time() * 1000)
        return ticks
        
    def elapsed(self):
        if sys.implementation.name == "micropython":
            return time.ticks_diff(self.now(), self._ticks)
        return self.now() - self._ticks
        
    def isExpired(self):
        return self.elapsed() > self._timeout
    
########################################################################
# ORCP_Config holds information for configuration parts of the CFG MSG #
########################################################################
class ORCP_Config():

    def __init__(self, cfg_type, data_len, timeout, priority, id_key, side):
        self._conf_len  = 7 + _DGT_4LEN * 3 + _DGT_6LEN + len(id_key)
        self._callback  = self.callbackFromIdKey(id_key, side)
        self._conf_type = cfg_type
        self._data_len  = data_len
        self._timeout   = TimeOut(timeout)
        self._priority  = priority
        self._id_key    = id_key
        
    def is_timed_out(self):
        return self._timeout.isExpired()
        
    def get_str_conf_type(self):
        return (self._conf_type) 
        
    def get_str_conf_len(self):
        return ('000000' + str(self._conf_len))[-_DGT_4LEN:]      
        
    def get_str_data_len(self):
        return ('000000' + str(self._data_len))[-_DGT_4LEN:]      
        
    def get_str_timeout(self):
        return ('000000' + str(self._timeout.timeout()))[-_DGT_6LEN:]      
        
    def get_str_priority(self):
        return ('000000' + str(self._priority))[-_DGT_4LEN:]      
        
    def get_as_string(self):
        result = ''
        result += self.get_str_conf_len() + '|'
        result += self.get_str_conf_type() + '|'
        result += self.get_str_data_len() + '|'
        result += self.get_str_timeout() + '|'
        result += self.get_str_priority() + '|'
        result += self._id_key + '|'
        return result
        
    def __str__(self):
        result = ''
        result += 'config type:' + self.get_str_conf_type() + '\n'
        result += 'config len:' + self.get_str_conf_len() + '\n'
        result += 'data len:' + self.get_str_data_len() + '\n'
        result += 'timeout:' + self.get_str_timeout() + '\n'
        result += 'priority:' + self.get_str_priority() + '\n'
        result += 'id key:' + self._id_key + '\n'
        return result
        
    ####################################################################
    # ORCP_Config has methods to read or write data to the config data #
    ####################################################################

    # _ROBOT side callbacks
    def driveTrainActuatorToRobot(self):
        print("driveTrainActuatorToRobot(self)")
        # Drive train data is movement, turn & speed
        # Movement = [Forward, Backward, Left, Right, None]
        # Direction = [Left, Right, None]
        # Speed = [0 - 255]
        # Example: |BackWard  Right     255|

        return self._data_len * 'A'

    def GPSModuleSensorFromRobot(self):
        print("GPSModuleSensorFromRobot(self)")
        return self._data_len * 'B'

    def BumpersFromRobot(self):
        print("BumpersFromRobot(self)")
        return self._data_len * 'C'

    # _BRAIN side callbacks
    def driveTrainActuatorFromBrain(self):
        print("driveTrainActuatorFromBrain(self)")
        # Drive train data is movement, turn & speed
        # Movement = [Forward, Backward, Left, Right, None]
        # Direction = [Left, Right, None]
        # Speed = [0 - 255]
        # Example: |BackWard  Right     255|
        command = "BackWard  Right     255"
        print("DRIVE_TRAIN:", "["+command[0:self._data_len]+"]")
        return command[0:self._data_len]

    def GPSModuleSensorToBrain(self):
        print("GPSModuleSensorToBrain(self)")
        return self._data_len * 'B'

    def BumpersToBrain(self):
        print("BumpersToBrain(self)")
        return self._data_len * 'C'

    # dummy callback just in case...
    def default_callback(self):
        print("default_callback(self)")
        return self._data_len * 'X'

    def callbackFromIdKey(self, id_key, link_side):
        if link_side == _ROBOT:        
            if id_key == 'DriveTrain': 
                return self.driveTrainActuatorToRobot
            if id_key == 'GPS_Module': 
                return self.GPSModuleSensorFromRobot
            if id_key == 'Bumpers': 
                return self.BumpersFromRobot
        if link_side == _BRAIN:
            if id_key == 'DriveTrain': 
                return self.driveTrainActuatorFromBrain
            if id_key == 'GPS_Module': 
                return self.GPSModuleSensorToBrain
            if id_key == 'Bumpers': 
                return self.BumpersToBrain
        return self.default_callback
        
########################################################################
# Setting up the real hardware modules                                 #
########################################################################
       
configuration = ( 
    ORCP_Config('A', 23,  700, 1, 'DriveTrain', _ROBOT), 
    ORCP_Config('S', 24, 2100, 2, 'GPS_Module', _ROBOT), 
    ORCP_Config('S', 42,   50, 3, 'Bumpers', _ROBOT)     
)

########################################################################
# ORCP_Link contains the base functions for the Robot and the Brain.   #
########################################################################
class ORCP_Link():

    def __init__(self, sock, srv):
        """Initialize the link."""
        self._srv  = srv
        self._sock = sock
        self._msg_start = ''
        self._msg_init  = ''
        self._msg_hwcfg = ''
        self._msg_swcfg = ''
        self._msg_sense = ''
        self._msg_actnw = ''
        self._configs   = []

########################################################################
# Activate method, controlling the sequence of messages.               #
########################################################################

    def activate(self):
        # print('activate(self)')
        """Activate the link."""
        error = _OK
        if self._srv == _ROBOT:
            error = self.actAsRobot()
        else:
            error = self.actAsBrain()
        # print('return code of activate:', error)
        
    def actAsRobot(self):
        # print('actAsRobot(self)')
        for cfg in configuration:
            self.addConfig(cfg)

        # ROBOT starts by sending a START message
        if self.sendStartMsg() == _ERR:
            return _ERR
        if self.waitForInitMsg() == _ERR:
            return _ERR
        if self.processInitMsg() == _ERR:
            return _ERR
        if self.sendHwCfgMsg() == _ERR:
            return _ERR
        if self.waitForSwCfgMsg() == _ERR:
            return _ERR
        if self.processSwCfgMsg() == _ERR:
            return _ERR
        # print("Robot Setup complete!")
        return self.RobotSessionManager()
        
    def actAsBrain(self):
        # print('actAsBrain(self)')
        # BRAIN starts by listening for the START message
        if self.waitForStartMsg() == _ERR:
            return _ERR
        if self.processStartMsg() == _ERR:
            return _ERR
        if self.sendInitMsg() == _ERR:
            return _ERR
        if self.waitForHwCfgMsg() == _ERR:
            return _ERR
        if self.processHwCfgMsg() == _ERR:
            return _ERR
        if self.sendSwCfgMsg() == _ERR:
            return _ERR
        # print("Brain Setup complete!")
        return self.BrainSessionManager()

########################################################################
# ROBOT side methods, as they appear in the sequence.                  #
########################################################################

    def sendStartMsg(self):
        # print('sendStartMsg(self)')
        self._msg_start = _MSG_START
        self._send(self._msg_start)
        return _OK
    
    def waitForInitMsg(self):
        # print('waitForInitMsg(self)')
        self._msg_init = self._recv()
        result = self._chk_msg(self._msg_init, _MSG_INIT)
        return result
    
    def processInitMsg(self):
        # print('processInitMsg(self)')
        return _OK
    
    def sendHwCfgMsg(self):
        # print('sendHwCfgMsg(self)')
        self._msg_hwcfg = _MSG_HWCFG
        for cfg in self._configs:
            self._msg_hwcfg += cfg.get_as_string()
        self._msg_hwcfg += _MSG_CFG_0
        self._send(self._msg_hwcfg)
        return _OK
    
    def waitForSwCfgMsg(self):
        # print('waitForSwCfgMsg(self)')
        self._msg_swcfg = self._recv()
        result = self._chk_msg(self._msg_swcfg, _MSG_SWCFG)
        return result
    
    def processSwCfgMsg(self):
        # print('processSwCfgMsg(self)')
        return _OK
        
    def addConfig(self, new_config):
        # print('addConfig(self, new_config)')
        self._configs.append(new_config)

    def RobotSessionManager(self):
        # print('RobotSessionManager(self)')
        # ROBOT sends sensory messages, and receives actuator messages
        result = _OK
        while result == _OK:
           result = self.sendNextSensoryMessage()
           if result == _ERR:
               break
           result = self.processNextActuatorMessage()
           if result == _ERR:
               break
        return result
    
    def sendNextSensoryMessage(self):
        # print('sendNextSensoryMessage(self)')
        # send first expired message, or highest priority message
        result = _OK
        msg_sent = False
        for cfg in self._configs:
            if cfg.get_str_conf_type() == 'S':
                if cfg.is_timed_out():
                    self.sendSensoryMsg(cfg)                  
                    msg_sent = True
        if msg_sent == False:
            self._send(_MSG_NOOP)
        return result
    
    def processNextActuatorMessage(self):
        # print('processNextActuatorMessage(self)')
        result = _OK
        self._msg_actnw = self._recv()
        if self._chk_msg(self._msg_actnw, _MSG_NOOP) == _OK:
            return _OK
        result = self._chk_msg(self._msg_actnw, _MSG_ACTNW)
        return result

    def sendSensoryMsg(self, cfg):
        # print('sendSensoryMsg(self, cfg)')
        self._msg_sense = _MSG_SENSE
        self._msg_sense += cfg._callback()
        self._send(self._msg_sense)
        cfg._timeout.reset()
        return _OK
    
    
########################################################################
# BRAIN side methods, as they appear in the sequence.                  #
########################################################################

    def waitForStartMsg(self):
        # print('waitForStartMsg(self)')
        self._msg_start = self._recv()    
        result = self._chk_msg(self._msg_start, _MSG_START)
        return result

    def processStartMsg(self):
        # print('processStartMsg(self)')
        return _OK
    
    def sendInitMsg(self):
        # print('sendInitMsg(self)')
        self._msg_init = _MSG_INIT
        return self._send(self._msg_init)
    
    def waitForHwCfgMsg(self):
        # print('waitForHwCfgMsg(self)')
        self._msg_hwcfg = self._recv()    
        result = self._chk_msg(self._msg_hwcfg, _MSG_HWCFG)
        return result

    def processHwCfgMsg(self):
        # print('processHwCfgMsg(self)')
        conf_str = self._msg_hwcfg[6:]
        while conf_str != '0000|':
            conf_len = int(conf_str[0:_DGT_4LEN])
            left_len = conf_len - _DGT_4LEN - 1
            conf_type = conf_str[5:6]
            left_len -= 2
            data_len = int(conf_str[7:7 + _DGT_4LEN])
            left_len -= (_DGT_4LEN + 1)
            timeout = int(conf_str[12:12 + _DGT_6LEN])
            left_len -= (_DGT_6LEN + 1)
            priority = int(conf_str[19:19 + _DGT_4LEN])
            left_len -= (_DGT_4LEN + 1)
            id_key = conf_str[24:24 + left_len - 1]
            conf_str = conf_str[24 + left_len:]
            config = ORCP_Config(conf_type, data_len, timeout, priority, id_key, _BRAIN)
            self.addConfig(config)
        return _OK
    
    def sendSwCfgMsg(self):
        # print('sendSwCfgMsg(self)')
        # TODO: processing before sending
        self._msg_swcfg = _MSG_SWCFG
        for cfg in self._configs:
            self._msg_swcfg += cfg.get_as_string()
        self._msg_swcfg += _MSG_CFG_0
        return self._send(self._msg_swcfg)

    def BrainSessionManager(self):
        # print('BrainSessionManager(self)')
        # BRAIN receives sensory messages and sends actuator messages
        result = _OK
        while result == _OK:
           # print('recv sensor data')
           result = self.processNextSensoryMessage()
           if result == _ERR:
               break
           # print('send actuator data')
           result = self.sendNextActuatorMessage()
           if result == _ERR:
               break
        return result
    
    def processNextSensoryMessage(self):
        # print('processNextSensoryMessage(self)')
        # process the next incoming message, if it is a sensory message
        result = _OK
        self._msg_sense = self._recv()
        if self._chk_msg(self._msg_sense, _MSG_NOOP) == _OK:
            return _OK
        result = self._chk_msg(self._msg_sense, _MSG_SENSE)
        return result
    
    def sendNextActuatorMessage(self):
        # print('sendNextActuatorMessage(self)')
        # send first expired message, or highest priority message
        result = _OK
        msg_sent = False
        for cfg in self._configs:
            if cfg.get_str_conf_type() == 'A':
                if cfg.is_timed_out():
                    self.sendActuatorMsg(cfg)
                    msg_sent = True
        if msg_sent == False:
            self._send(_MSG_NOOP)
        return result

    def sendActuatorMsg(self, cfg):
        # print('sendActuatorMsg(self, cfg)')
        self._msg_actnw = _MSG_ACTNW
        self._msg_actnw += cfg._callback()
        self._send(self._msg_actnw)
        cfg._timeout.reset()
        return _OK
    
    
########################################################################
# Generic internal methods for sending, receiving and other things     #
########################################################################

    def _send(self, msg):
        """Send a message as a length-specified message."""
        msg_len = len(msg)
        len_str = ('00000' + str(msg_len))[-4:]
        len_plus_msg = '|' + str(len_str) + '|' + str(msg)
        print('>>', len_plus_msg)
        arr = bytes(len_plus_msg, 'UTF-8')
        self._sock.send(arr)

    def _recv(self):
        """Receive a length-specified message and return it."""
        msg_len = b''
        while msg_len == b'':
            msg_len = self._sock.recv(_MSG_LEN + 2)
            time.sleep(0.1)
        len_bytes = str(msg_len[1:5], 'UTF-8')
        msg = self._sock.recv(int(len_bytes))
        msg = str(msg, 'UTF-8')
        print('<<', '|' + len_bytes + '|' + msg)
        return msg
  
    def _chk_msg(self, msg, msg_type):
        """Check if a message is the right message type."""
        msg = msg[0:5]
        msg_type = msg_type[0:5]
        if msg == msg_type:
            return _OK
        return _ERR
    
    def __str__(self):
        result  = '_svr: ' + str(self._srv) + '\n'
        result += '_sock: ' + str(self._srv) + '\n'
        if self._msg_start is not None:
            result += '_msg_start: ' + self._msg_start + '\n'
        if self._msg_init is not None:
            result += '_msg_init: ' + self._msg_init + '\n'
        if self._msg_hwcfg is not None:
            result += '_msg_hwcfg: ' + self._msg_hwcfg + '\n'
        if self._msg_swcfg is not None:
            result += '_msg_swcfg: ' + self._msg_swcfg + '\n'
        if self._msg_sense is not None:
            result += '_msg_sense: ' + self._msg_sense + '\n'
        if self._msg_actnw is not None:
            result += '_msg_actnw: ' + self._msg_actnw + '\n'
        return result
