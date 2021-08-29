Open Robot Communications Protocol

This folder contains both the classes of the ORCP library handy, 
as well as the Python file for the PC client and the files for the 
MicroPython server side. 

- all orcp_[*].py files belong to the library, and are used by both sides.
- boot.py and main.py are the files needed on the MicroPython server side.
- control.py is the file to execute the setup of the link to the robot.

The connection process is as follows:

- the PC is connected to the local network either by cable or WiFi.
- the Robot ESP32 board uses boot.py to log into WiFi on the local network.
- when connected to WiFi, main.py will set up the server for the ORCP linkup.
- if it is ready, control.py can be used on the PC to set up a link.
- PC will then go to receive mode, waiting for robot to respond.
- once connected, PC receives Startup message from robot.
- PC replies with Initialize message if Link Identifier matches.
- Robot reacts with Hardware Configuration message.
- PC maps configuration to its software, replies with SW Configuration message.
- From here, link is set up, and normal traffic commences.
- Robot sends Sensory messages, PC sends Actuator messages.


