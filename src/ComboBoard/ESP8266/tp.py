link_id = '20211011121314'

# topic strings
t_s   = b'Startup'
t_a   = b'Acquire'
t_hc  = b'HWConfig'
t_sc  = b'SWConfig'
t_sns = b'Sensor'
t_act = b'Actuator'

# sensors
s_ir    = 'InfraRed'
s_ws    = 'WheelSensor'
s_is    = 'InertialSensor'

# actuators
a_arm   = 'Arm'
a_wrist = 'Wrist'
a_hand  = 'Hand'
a_cam   = 'Camera'
a_md    = 'MotorDrive'

# end of config label
c_end   = b'EndConfig'
    
# operating modes
m_u  = 'unconnected'
m_a  = 'acquiring'
m_c  = 'configuring'
m_o  = 'operating'

# config lists
sens_list = [s_ir, s_ws, s_is]
actr_list = [a_arm, a_wrist, a_hand, a_cam, a_md]