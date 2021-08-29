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
_MSG_CFG_0 = '0000|'
  
########################################################################
# ORCP_Link contains the base functions for the Robot and the Brain.   #
########################################################################
class ORCP_Link():

    def __init__(self, sock, srv):
        """Initialize the link."""
        self._srv  = srv
        self._sock = sock
        self._msg_start = None
        self._msg_init  = None
        self._msg_hwcfg = None
        self._msg_swcfg = None
        self._msg_sense = None
        self._msg_actnw = None
        self._configs   = []

########################################################################
# Activate method, controlling the sequence of messages.               #
########################################################################

    def activate(self):
        """Activate the link."""
        # start with 'No Error'...
        error = _OK
        if self._srv == _ROBOT:
            error = self.actAsRobot()
        else:
            error = self.actAsBrain()
        print('return code of activate:', error)
        
    def actAsRobot(self):
        self.addConfig(ORCP_Config('A', 17, 100, 1, 'DriveTrain'))
        self.addConfig(ORCP_Config('S', 24, 1100, 2, 'GPS_Module'))
        self.addConfig(ORCP_Config('A', 42, 555, 3, 'Bumpers'))
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
        print("Robot Setup complete!")
        
    def actAsBrain(self):
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
        print("Brain Setup complete!")


########################################################################
# ROBOT side methods, as they appear in the sequence.                  #
########################################################################

    def sendStartMsg(self):
        self._msg_start = _MSG_START
        # TODO: processing before sending
        self._send(self._msg_start)
        print('>>MSG:', self._msg_start)
        return _OK
    
    def waitForInitMsg(self):
        self._msg_init = self._recv()
        # TODO: processing after receiving
        result = self._chk_msg(self._msg_init, _MSG_INIT)
        print('<<MSG:', self._msg_init)
        return result
    
    def processInitMsg(self):
        # TODO: processing of the stored message
        return _OK
    
    def sendHwCfgMsg(self):
        self._msg_hwcfg = _MSG_HWCFG
        # TODO: processing before sending
        for cfg in self._configs:
            self._msg_hwcfg += cfg.get_as_string()
        self._msg_hwcfg += _MSG_CFG_0
        self._send(self._msg_hwcfg)
        print('>>MSG:', self._msg_hwcfg)
        return _OK
    
    def waitForSwCfgMsg(self):
        self._msg_swcfg = self._recv()
        # TODO: processing after receiving
        result = self._chk_msg(self._msg_swcfg, _MSG_SWCFG)
        print('<<MSG:', self._msg_swcfg)
        return result
    
    def processSwCfgMsg(self):
        # TODO: processing of the stored message
        return _OK
        
    def addConfig(self, new_config):
        self._configs.append(new_config)


########################################################################
# BRAIN side methods, as they appear in the sequence.                  #
########################################################################

    def waitForStartMsg(self):
        self._msg_start = self._recv()    
        # TODO: processing after receiving
        result = self._chk_msg(self._msg_start, _MSG_START)
        print('<<MSG:', self._msg_start)
        return result

    def processStartMsg(self):
        # TODO: processing of the stored message
        return _OK
    
    def sendInitMsg(self):
        self._msg_init = _MSG_INIT
        # TODO: processing before sending
        print('>>MSG:', self._msg_init)
        return self._send(self._msg_init)
    
    def waitForHwCfgMsg(self):
        self._msg_hwcfg = self._recv()    
        # TODO: processing after receiving
        result = self._chk_msg(self._msg_hwcfg, _MSG_HWCFG)
        print('<<MSG:', self._msg_hwcfg)
        return result

    def processHwCfgMsg(self):
        # TODO: processing of the stored message
        conf_str = self._msg_hwcfg[6:]
        while conf_str != '0000|':
            conf_len = int(conf_str[0:_DGT_4LEN])
            left_len = conf_len - _DGT_4LEN - 1
            conf_type = conf_str[5:0]
            left_len -= 2
            data_len = int(conf_str[7:7 + _DGT_4LEN])
            left_len -= (_DGT_4LEN + 1)
            timeout = int(conf_str[12:12 + _DGT_6LEN])
            left_len -= (_DGT_6LEN + 1)
            priority = int(conf_str[19:19 + _DGT_4LEN])
            left_len -= (_DGT_4LEN + 1)
            id_key = conf_str[24:24 + left_len - 1]
            conf_str = conf_str[24 + left_len:]
            config = ORCP_Config(conf_type, data_len, timeout, priority, id_key)
            self.addConfig(config)
        return _OK
    
    def sendSwCfgMsg(self):
        self._msg_swcfg = _MSG_SWCFG
        # TODO: processing before sending
        for cfg in self._configs:
            self._msg_swcfg += cfg.get_as_string()
        self._msg_swcfg += _MSG_CFG_0
        print('>>MSG:', self._msg_swcfg)
        return self._send(self._msg_swcfg)

########################################################################
# Generic internal methods for sending, receiving and other things     #
########################################################################

    def _send(self, msg):
        """Send a message as a length-specified message."""
        msg_len = len(msg)
        len_str = ('00000' + str(msg_len))[-4:]
        len_plus_msg = '|' + str(len_str) + '|' + str(msg)
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
        return msg
  
    def _chk_msg(self, msg, msg_type):
        """Check if a message is the right message type."""
        msg = msg[0:5]
        msg_type = msg_type[0:5]
        if msg == msg_type:
            return _OK
        return _ERR
    
    def __str__(self):
        '''Put all attributes in a string.'''
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

########################################################################
# ORCP_Config holds information for configuration parts of the CFG MSG #
########################################################################
class ORCP_Config():

    def __init__(self, cfg_type, data_len, timeout, priority, id_key):
        self._conf_len  = 7 + _DGT_4LEN * 3 + _DGT_6LEN + len(id_key)
        self._conf_type = cfg_type   # len 1
        self._data_len  = data_len   # _DGT_4LEN
        self._timeout   = timeout    # _DGT_6LEN
        self._priority  = priority   # _DGT_4LEN
        self._id_key    = id_key     # len(str)

    def get_str_conf_type(self):
        return (self._conf_type) 
        
    def get_str_conf_len(self):
        return ('000000' + str(self._conf_len))[-_DGT_4LEN:]      
        
    def get_str_data_len(self):
        return ('000000' + str(self._data_len))[-_DGT_4LEN:]      
        
    def get_str_timeout(self):
        return ('000000' + str(self._timeout))[-_DGT_6LEN:]      
        
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
    