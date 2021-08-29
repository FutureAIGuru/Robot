INSTALLATION OF THE DEBUG DISPLAY

First of all, set up the hardware according to the image "Wiring Diagram LCD.jpg". Then connect the Arduino to the USB of your PC. The display may show some random characters, but this is normal. 

If needed, install the Arduino IDE and set the board and port in the tools menu.

Next, open up the I2C_Scanner sketch in the Arduino IDE. It's place on the web is https://playground.arduino.cc/Main/I2cScanner/, but it was included here for convenience. Once you have compiled the scanner and sent it to the Arduino board, open the serial console of the IDE and check its output. This will give you the address of the LCD display (make sure only the display is connected).

With the Arduino IDE still open, select "Sketch => Include Library => Add .ZIP Library..." and select the LCD_I2C.zip from this folder. This adds the needed functionality to be included in the LC_DEBUG program.

Next, open the LC_DEBUG sketch, and make sure the 'addr' constant at line 3 is identical to the value the I2C_Scanner program reported back.

The default LC_DEBUG program sends lines received from the Serial port to "printMessageToLCD(String line)" which chops them into 20 character parts, and sends them as lines towards the display using "lcd.addLine(line);". The serial console of the Arduino IDE limits the line to 63 characters, but if we were calling the printMessageToLCD() function from any other source, the length can be of arbitrary length, although the first part of any string over 80 characters in length will be scrolled off the top of the screen so the last 80 characters will only be visible.

While it is possible to send characters using the Arduino console window (set to 9600 baud and 'no line endings', the Python script SEND_2_SERIAL.py has been added to give an example of how something similar might be done from a program other than the Arduino IDE. It basically allows the user to send string to the serial port, and types out messages sent back.

