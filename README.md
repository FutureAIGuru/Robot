# Hardware Overview

For the tiny brains of this robot the following board was used, which combines an Arduino MEGA 2560 with an ESP8266 processor. It can be ordered [here ]([Geekcreit mega +wifi r3 module atmega2560+esp8266 32mb memory usb-ttl ch340g Sale - Banggood.com](https://www.banggood.com/Geekcreit-Mega-+WiFi-R3-Module-ATmega2560+ESP8266-32Mb-Memory-USB-TTL-CH340G-p-1205437.html)) on banggood.com. As a result, the code to be installed on it is divided into two folders, named `Arduino`  and `ESP8266`. 

To set up the various configurations of the board, a set of 8 tiny DIP switches are present, which work as follows:

| Connection           | DIP1 | DIP2 | DIP3 | DIP4 | DIP5 | DIP6 | DIP7 |
| -------------------- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| ATMega2560<=>ESP8266 | ON   | ON   | OFF  | OFF  | OFF  | OFF  | OFF  |
| USB<=>ATMega2560     | OFF  | OFF  | ON   | ON   | OFF  | OFF  | OFF  |
| USB<=>ESP8266 upd    | OFF  | OFF  | OFF  | OFF  | ON   | ON   | ON   |
| USB<=>ESP8266 com    | OFF  | OFF  | OFF  | OFF  | ON   | ON   | OFF  |
| All independent      | OFF  | OFF  | OFF  | OFF  | OFF  | OFF  | OFF  |

In the following description, these configurations will be marked as binary numbers with an X at the end, since DIP switch number 8 is not used. 

## Arduino MEGA 2560

For programming of the Arduino Mega set configuration 0011000X. In this setup, the board can easily be programmed using the programs from [arduino.cc](https://arduino.cc). Some extra actions are needed however, since the Arduino IDE is by default not configured for ESP boards. 

Start the Arduino IDE, choose File => Preferences, and set the following values in the `Additional Board Manager URLs` field:

https://dl.espressif.com/dl/package_esp32_index.json,
http://arduino.esp8266.com/stable/package_esp8266com_index.json

After this, selecting Tools => Board will show three categories, where AVR, ESP32 and ESP8266 boards can be selected. For our board, select `Generic ESP8266 Module` from the ESP8266 Boards category.

## ESP8266

The ESP8266 runs MicroPython, and uses MQTT to communicate with another MQTT client on the PC that runs Brain Simulator II. The MQTT server (a.k.a. broker) can run anywhere on the network. 

The board of course comes without MicroPython pre-installed, but the procedure to install it is quite simple:

- You will need Espressif's esptool to get the software onto the ESP8266. This can be downloaded [here]([GitHub - espressif/esptool: Espressif SoC serial bootloader utility](https://github.com/espressif/esptool)). 

- Set the board config switches to 0000111X for ESP8266 programming mode and connect it to your computer. 

- Start the Arduino IDE to identify the proper port the ESP8266 is connected to. 

- To download the latest version of MicroPython, [this page]([MicroPython - Python for microcontrollers](https://micropython.org/download/esp8266/)) is recommended. 

- Information on installing MicroPython can be found on this [page]([1. Getting started with MicroPython on the ESP8266 &mdash; MicroPython 1.17 documentation](https://docs.micropython.org/en/latest/esp8266/tutorial/intro.html)), but the short version is below.

- Clear the board with `python esptool.py --port COM erase_flash`, where COM is replaced with the actual com port identified earlier.

- Load MicroPython with `python esptool.py --port COM --baud 460800 write_flash --flash_size=detect 0 esp8266?.bin`, where COM is replaced with the actual com port, and esp8266?.bin is the name of the MicroPython file downloaded in step four.

- To check the installation, set the board config switches to 0000110X for ESP8266 communication mode, and start a terminal program like Putty to make a serial connection to the board (settings 115200 baud, 8 bits data, 1 stop bit no parity and no flow control).

- If the board replies with the version information of MicroPython and the familiar `>>>` prompt, you have succesfully loaded MicroPython on the board.

## Getting the Robot software

The source code for the Robot project can be found on Github, right [here]([GitHub - FutureAIGuru/Robot](https://github.com/FutureAIGuru/Robot)). You can either get it through Git or the Github Desktop app, but for the purpose of just buidling the robot, it will suffice to just download the code as a ZIP file and unpack it. Of course, if you are reading this, you may have already done this...

The top level of the project is split in a `doc` folder, an `experiments` folder, and the `src` folder for the main software package. The src folder has been further subdivided into the folders `Arduino`, and the `ESP8266` folder for the two halves of our robot board. 

## Loading the software on the ESP8266

To load the software onto the MicroPython  ESP8266, a package named rshell is the most appropriate. The sources for this can be found [here]([GitHub - dhylands/rshell: Remote Shell for MicroPython](https://github.com/dhylands/rshell)), but the simplest way of installing it is with the command `pip install rshell`.

rshell is a command line tool, with command much like the common linux command (ls, cp, etc). When invoked, the very first command should always be the setup to connect to a board. But we will go through this step by step:

- First of all set the board switches to 0000111X for ESP8266 programming mode.

- Connect the board and make sure you know the COM port it uses.

- Open a command line prompt, and change to the `src/ESP8266` folder.

- invoke rshell, and after the startup message type `connect serial COM`, where COM is the port the board is connected to.

- The command produces several lines of output, one of which reports the name to be pyboard. This will be changed in the following steps.

- open the board.py file and put the name you want there.  By default, the name is set to `robot`. Advice is to keep it short and without spaces or nasty characters.

- type the command `cp board.py /pyboard/` to copy the board.py file to the board.

- quit rshell and reset the board.

- restart rshell and connect again, name should now be whatever you put in the board.py file. We will assume the name to be robot in the following steps.

- issue the rshell command `ls /robot/` at the rshell prompt. It will show two files, named `board.py` and `boot.py`.

- first replace the boot.py file by the one in our project, with `cp boot.py /robot/`.

- next do the same with `cp main.py /robot/` and `cp umqttsimple.py /robot/`.

- last, issue the `ls -l /robot/` command to verify all files are on the board.

- Now the serial terminal can be started again, and the board reset, and it will show how the board connects to the WiFi. 

Of course the SSID and WiFi password need to be set, but the software will take care of that if no settings are present yet. The procedure is as follows:

- With the board connected, start the terminal again.

- Press the reset button to restart the board.

- You will be asked for the SSID of the network, enter it, and confirm the value

- Next is the password, enter it, and confirm.

- Third value is the MQTT server IP address, where you are running the server.

- Last is the Client ID of the board, set it to something unique.

- After that, the software will try to connect to the WiFi first, and the MQTT broker next. 

- If connectionns are not established, the reset of the configuration can be forced by connecting two pins on the board and resetting it. 

![](C:\Users\Moorelife\Desktop\board.png)

All that is required for the reset is to connect the two pins marked in red in the above image, and reset the board. As soon as you have set all configuration values, remove the connection and the software will attempt to connect again. 
