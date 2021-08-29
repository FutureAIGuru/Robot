# LCD_I2C
When open source software does not do what you would want it to do, you have the option to adapt it, or to go back to the roots and do it to your own liking. Being a software test engineer, which still involves a lot of programming too, I started testing the LiquidCrystal_I2C library, and found a few minor inconsistencies with it:

1. There were pairs of similar methods to switch certain stuff on and off, rather than one method to switch the same thing based on a boolean. I decided to change the API by replacing the old functions with switchBacklight(bool), switchDisplay(bool), switchCursor(bool) and switchBlink(bool).

2. The LCD controller used in the 1602 and 2004 LCD displays is not entirely straightforward: sending 32 characters to a 1602 correctly displays it as two consecutive lines, but sending 80 characters to a 2004 display incorrectly displays it as line #1, line #3, line #2 and line #4. That definitely needed improvement. 

3. Rather than using the built in scroll mechanism of the LCD controller, which seemed to work in mysterious ways, I decided to implement scrollUp(), scrollDown(), scrollLeft() and scrollRight(). Might change this to become scrollVertical(direction
