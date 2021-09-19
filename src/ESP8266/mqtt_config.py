import struct as s

# topic: 'Config'
# - config_id 
# - data length
# - timeout
q_config = b'Config' # topic string
s_config = '20sii'   # pack string   

def pack_config(config_id, data_len, timeout):
    return s.pack(s_config, config_id, data_len, timeout)

def unpack_drive(pkg):
    return s.unpack(s_config, pkg)

# topic: 'Drive'
# - Move = ['Forward', 'Backward', 'ShiftLeft', 'ShiftRight']
# - Turn = ['SpinLeft', 'SpinRight', 'TurnLeft', 'TurnRight']
# - Speed = [0-255]
q_drive = b'Drive'  # topic string
s_drive = '10s9si'  # pack string

def pack_drive(move, turn, speed):
    return s.pack(s_drive, move, turn, speed)

def unpack_drive(pkg):
    return s.unpack(s_drive, pkg)

# topic: 'Wheels'
# - FrontLeft = [0-1000]
# - FrontRight = [0-1000]
# - RearLeft = [0-1000]
# - RearRight = [0-1000]
q_wheels = b'Wheels'  # topic string
s_wheels = 'iiii'     # pack string

def pack_wheels(fl, fr, rl, rr):
    return s.pack(s_wheels, fl, fr, rl, rr)

def unpack_wheels(pkg):
    return s.unpack(s_wheels, pkg)
