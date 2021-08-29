//******************************************************************************
// This is a reimplementation of the LiquidCrystal_I2C library which was
// originally based on the work by DFRobot, and which aims to be more user-
// friendly and thought out with the user of the library in mind.
//******************************************************************************
#include "LCD_I2C.h"
#include <inttypes.h>

String LCD_I2C::_empty = String("                    "); // max col_size

// overridden write to implement Print interface
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
	#define printIIC(args)	Wire.write(args)
	inline size_t LCD_I2C::write(uint8_t value) 
	{
		send(value, Rs);
		return 1;
	}
#else
	#include "WProgram.h"
	#define printIIC(args)	Wire.send(args)
	inline void LCD_I2C::write(uint8_t value) 
	{
		send(value, Rs);
	}
#endif
#include "Wire.h"

//*******************************************************************************
// When the display powers up, it is configured as follows:
//															
// 1. Display clear											
// 2. Function set:											
//		DL = 1; 8-bit interface data						
//		N = 0; 1-line display								
//		F = 0; 5x8 dot character font						
// 3. Display on/off control:								
//		D = 0; Display off									
//		C = 0; Cursor off									
//		B = 0; Blinking off									
// 4. Entry mode set:										
//		I/D = 1; Increment by 1								
//		S = 0; No shift										
//															
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the LCD_I2C
// constructor is called).													
//*******************************************************************************

//*******************************************************************************
// As opposed to LiquidCrystal_I2C, we specify rows first, then columns.	
//*******************************************************************************
LCD_I2C::LCD_I2C(uint8_t lcd_Addr,
				 uint8_t lcd_rows,
				 uint8_t lcd_cols)
{
	_Addr = lcd_Addr;
	_rows = lcd_rows;
	_cols = lcd_cols;
	_curr_row = 0;
	_curr_col = 0;
	_backlightval = LCD_NOBACKLIGHT;
	_auto_scroll = false;
}

void LCD_I2C::init()
{
	init_priv();
}

void LCD_I2C::init_priv()
{
	Wire.begin();
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	begin(_cols, _rows);	
}

void LCD_I2C::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) 
{
	if (lines > 1) 
	{
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lines;

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lines == 1)) 
	{
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	delay(50); 
	
	// Now we pull both RS and R/W low to begin commands
	expanderWrite(_backlightval);	// reset expander and turn backlight off (Bit 8=1)
	delay(1000);

		// put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	write4bits(0x03 << 4);
	delayMicroseconds(4500); // wait min 4.1ms

	// second try
	write4bits(0x03 << 4);
	delayMicroseconds(4500); // wait min 4.1ms

	// third go!
	write4bits(0x03 << 4); 
	delayMicroseconds(150);

	// finally, set to 4-bit interface
	write4bits(0x02 << 4); 

	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);	

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	switchDisplay(true);

	// clear it off
	clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);
	home();
}

//*******************************************************************************
// High Level Commands, for the User.				            			  
//*******************************************************************************

//*******************************************************************************
// Clears the screen.														
//*******************************************************************************
void LCD_I2C::clear()
{
	command(LCD_CLEARDISPLAY);	// clear display, set cursor position to zero
	delayMicroseconds(2000);		// this command takes a long time!
	setCursor(0, 0);
}

//*******************************************************************************
// Homes the cursor.
//*******************************************************************************
void LCD_I2C::home()
{
	command(LCD_RETURNHOME);	// set cursor position to zero
	delayMicroseconds(2000);	// this command takes a long time!
	_curr_row = 0;
	_curr_col = 0;
}

//*******************************************************************************
// Sets the cursor to a given row and column.
//*******************************************************************************
void LCD_I2C::setCursor(uint8_t row, uint8_t col)
{
	_curr_row = row;
	_curr_col = col;
	// Debug.print(DBG_INFO, "pos = %d, %d", _curr_row, _curr_col);

	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > _numlines) 
	{
		row = _numlines - 1;		// we count rows starting w/0
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

//*******************************************************************************
// Turns the Backlight on or off.												
//*******************************************************************************
void LCD_I2C::switchBacklight(bool state) 
{
	if (!state)
	{
		_backlightval = LCD_NOBACKLIGHT;
	}
	else
	{
		_backlightval = LCD_BACKLIGHT;
	}
	expanderWrite(0);
}

//*******************************************************************************
// Turns the Display on or off.													
//*******************************************************************************
void LCD_I2C::switchDisplay(bool state) 
{
	if (!state)
	{
		_displaycontrol &= ~LCD_DISPLAYON;
	}
	else
	{
		_displaycontrol |= LCD_DISPLAYON;
	}
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

//*******************************************************************************
// Turns the Cursor on or off.													
//*******************************************************************************
void LCD_I2C::switchCursor(bool state) 
{
	if (!state)
	{
		_displaycontrol &= ~LCD_CURSORON;
	}
	else
	{
		_displaycontrol |= LCD_CURSORON;
	}
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

//*******************************************************************************
// Turns Cursor Blinking on or off.												
//*******************************************************************************
void LCD_I2C::switchBlink(bool state) 
{
	if (!state)
	{
		_displaycontrol &= ~LCD_BLINKON;
	}
	else
	{
		_displaycontrol |= LCD_BLINKON;
	}
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

//*******************************************************************************
// Copies the passed in String to the internal screen buffer.
//*******************************************************************************
void LCD_I2C::setScreenBuffer(String screen)
{
	for (uint8_t r = 0; r <= _rows; r++)
	{
		_screenbuffer[r] = screen.substring(r*_cols, r*_cols + _cols);
	}
	setCursor(0, 0);
}

//*******************************************************************************
// Copies the internal buffer to the screen.									
//*******************************************************************************
void LCD_I2C::bufferToScreen() 
{
	for (uint8_t line_to_write = 0; line_to_write < _rows; line_to_write++)
	{
		lineToScreen(line_to_write);
	}
	setCursor(0, 0);
}

//*******************************************************************************
// Copies one line to the screen.									
//*******************************************************************************
void LCD_I2C::lineToScreen(uint8_t line_to_write) 
{
	setCursor(line_to_write, 0);
	print(_screenbuffer[line_to_write]);
	setCursor(line_to_write + 1, 0);
}

//*******************************************************************************
// Scrolls the screen up and adds the new line at the bottom.
//*******************************************************************************
void LCD_I2C::addLine(String line)
{
	String saved = _screenbuffer[0];
	for (uint8_t row = 0; row < _rows - 1; row++)
	{
		_screenbuffer[row] = _screenbuffer[row + 1];
	}
	String addLine = line + _empty;
	_screenbuffer[_rows - 1] = addLine.substring(0, _cols);
	bufferToScreen();
}

//*******************************************************************************
// Scrolls the screen up.
//*******************************************************************************
void LCD_I2C::scrollUp(bool rotate_data)
{
	String saved = _screenbuffer[0];
	for (uint8_t row = 0; row < _rows - 1; row++)
	{
		_screenbuffer[row] = _screenbuffer[row + 1];
	}
	if (rotate_data)
	{
		_screenbuffer[_rows - 1] = saved;
	}
	else
	{
		_screenbuffer[_rows - 1] = _empty.substring(0, _cols);
	}
	bufferToScreen();
}

//*******************************************************************************
// Scrolls the screen down.
//*******************************************************************************
void LCD_I2C::scrollDown(bool rotate_data)
{
	String saved = _screenbuffer[_rows - 1];
	for (uint8_t row = _rows - 1; row > 0; row--)
	{
		_screenbuffer[row] = _screenbuffer[row - 1];
	}
	if (rotate_data)
	{
		_screenbuffer[0] = saved;
	}
	else
	{
		_screenbuffer[0] = _empty.substring(0, _cols);
	}
	bufferToScreen();
}

//*******************************************************************************
// Scrolls the screen left.
//*******************************************************************************
void LCD_I2C::scrollLeft(bool rotate_data)
{
	for (uint8_t row = 0; row < _rows; row++)
	{
		String temp = _screenbuffer[row];
		_screenbuffer[row] = temp.substring(1);
		if (rotate_data)
		{
			_screenbuffer[row] += temp.substring(0, 1);
		}
		else
		{
			_screenbuffer[row] += " ";
		}
	}
	bufferToScreen();
}

//*******************************************************************************
// Scrolls the screen right.
//*******************************************************************************
void LCD_I2C::scrollRight(bool rotate_data)
{
	for (uint8_t row = 0; row < _rows; row++)
	{
		String temp = _screenbuffer[row];
		if (rotate_data)
		{
			_screenbuffer[row] = temp.substring(_cols - 1);
		}
		else
		{
			_screenbuffer[row] = " ";
		}
		_screenbuffer[row] += temp.substring(0, _cols - 1);
	}
	bufferToScreen();
}

//*******************************************************************************
// Allows us to fill the first 8 CGRAM locations with custom characters.		
//*******************************************************************************
void LCD_I2C::createChar(uint8_t location, uint8_t charmap[]) 
{
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++) 
	{
		write(charmap[i]);
	}
}

//*******************************************************************************
// createChar with PROGMEM input.												
// Example: const char bell[8] PROGMEM = {B00100, B01110, B01110, B01110,		
//									      B11111,B00000,B00100,B00000};			
//*******************************************************************************
void LCD_I2C::createChar(uint8_t location, const char *charmap) 
{
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++) 
	{
		write(pgm_read_byte_near(charmap++));
	}
}

/********** mid level commands, for sending data/cmds */

inline void LCD_I2C::command(uint8_t value) 
{
	send(value, 0);
}

//*******************************************************************************
// Corrects internal cursor position when write function is used.
//*******************************************************************************
bool LCD_I2C::_check_position(bool increment)
{
	bool reposition = false;
	if (increment)
	{
		_curr_col++;
	}
	if (_curr_col >= _cols)
	{
		_curr_col = 0;
		_curr_row++;
		reposition = true;
	}
	if (_curr_row >= _rows)
	{
		_curr_row = 0;
		reposition = true;
	}
	if (reposition)
	{
		setCursor(_curr_row, _curr_col);
	}
}
	
//*******************************************************************************
// Low Level data pushing commands.
//*******************************************************************************

//*******************************************************************************
// Write either command or data.
//*******************************************************************************
void LCD_I2C::send(uint8_t value, uint8_t mode) 
{
	uint8_t hi_nibble = value & 0xf0;
	uint8_t lo_nibble = (value << 4) & 0xf0;
	write4bits((hi_nibble) | mode);
	write4bits((lo_nibble) | mode); 
	if (mode == Rs)
	{
		_check_position(true);
	}
}

//*******************************************************************************
// Write four bits through the Expander to the LCD Controller.
//*******************************************************************************
void LCD_I2C::write4bits(uint8_t value) 
{
	expanderWrite(value);
	pulseEnable(value);
}

//*******************************************************************************
// Write data to the I2C Expander board.
//*******************************************************************************
void LCD_I2C::expanderWrite(uint8_t _data)
{																				
	Wire.beginTransmission(_Addr);
	printIIC((int)(_data) | _backlightval);
	Wire.endTransmission();	 
}

//*******************************************************************************
// Pulse the Enable signal so the Expander shoves its data into the LCD.
//*******************************************************************************
void LCD_I2C::pulseEnable(uint8_t _data)
{
	expanderWrite(_data | En);	// En high.
	delayMicroseconds(1);		// enable pulse must be > 450 nanoseconds.
	
	expanderWrite(_data & ~En);	// En low.
	delayMicroseconds(50);		// commands need > 37 microseconds to settle.
} 

//*******************************************************************************
// Unsupported API functions.							
//*******************************************************************************
/*
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void LCD_I2C::off()
{
}

void LCD_I2C::on()
{
}

void LCD_I2C::setDelay (int cmdDelay,int charDelay)
{
}

uint8_t LCD_I2C::status()
{
	return 0;
}

uint8_t LCD_I2C::keypad ()
{
	return 0;
}

uint8_t LCD_I2C::init_bargraph(uint8_t graphtype)
{
	return 0;
}

void LCD_I2C::draw_horizontal_graph(uint8_t row, 
											uint8_t column, 
									uint8_t len,
									uint8_t pixel_col_end)
{
}

void LCD_I2C::draw_vertical_graph(uint8_t row, 
									uint8_t column,
									uint8_t len,
									uint8_t pixel_row_end)
{
}

void LCD_I2C::setContrast(uint8_t new_val)
{
}

#pragma GCC diagnostic pop
*/
