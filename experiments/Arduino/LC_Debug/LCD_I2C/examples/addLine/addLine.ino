#include <LCD_I2C.h>

const uint8_t rows = 4;
const uint8_t cols = 20;

LCD_I2C lcd = LCD_I2C(0x27, rows, cols);

String full_screen = "Testing the LCD_I2C |===================ABCDEFGHIJKLMNOPQRST0123456789+_/*()[]{}";
String add_lines =   "Adding a line.      Adding another line ... and another.    and the last one.   ";

long start = millis();
uint8_t offset = 0;

void setup() 
{
	// put your setup code here, to run once:
	Serial.begin(9600);
	lcd.init();
	lcd.switchBacklight(true);
	lcd.switchDisplay(true);
	lcd.switchCursor(false);
	lcd.switchBlink(false);

	// lcd.setScreenBuffer(full_screen);
	// lcd.bufferToScreen();	
}

void loop() 
{
	// reply only when you receive data:
	if (millis() - start > 2000)
	{
		start = millis();
		lcd.addLine(add_lines.substring(offset, offset + cols));
		offset += cols;
		if (offset >= rows * cols)
		{ 
			offset = 0;
		}
	}
}