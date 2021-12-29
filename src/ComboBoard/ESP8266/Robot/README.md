# Robot Arduino MEGA 2650 code

This section of the Robot repository contains the Arduino MEGA 2650 code required to process the Configuration strings the ModuleRobot module in Brain Simulator II sends, as well as handling the Actuator strings and Sensor strings that get communicated to and from it via the Serial channel used to program it in normal Arduino configuration of the ComboBoard.

By setting the ComboBoard to the special mode where USB is chained to Arduino and Arduino to ESP8266, and selecting the RXD0/TXD0 channel for the communication between the Arduino and the ESP8266, we can leave this code unchanged, and still have the ESP8266 relay the data over WiFi with TCP/IP. 
