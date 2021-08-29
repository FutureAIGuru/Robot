//******************************************************************************
// This is a reimplementation of the LiquidCrystal_I2C library which was	
// originally based on the work by DFRobot, and which aims to be more user-	
// friendly and thought out with the user of the library in mind.			
//******************************************************************************
#ifndef LCD_I2C_h
#define LCD_I2C_h

#include <inttypes.h>
#include "Print.h" 
#include <Wire.h>

#include "LCD_Constants.h"

class LCD_I2C : public Print 
{
public:
	LCD_I2C(uint8_t lcd_Addr,
			uint8_t lcd_rows,
			uint8_t lcd_cols);

	// overridden write to implement Print interface
	#if defined(ARDUINO) && ARDUINO >= 100
	virtual size_t write(uint8_t);
	#else
	virtual void write(uint8_t);
	#endif
	void command(uint8_t);

	void init();
	void begin(uint8_t cols, 
				 uint8_t rows, 
				 uint8_t charsize = LCD_5x8DOTS );
				 
	//******************************************************************************
	// High Level Commands, for the User.										
	//******************************************************************************
	void clear();
	void home();
	
	void setCursor(uint8_t row, uint8_t col);

	void switchBacklight(bool state);
	void switchDisplay(bool state);
	void switchCursor(bool state);
	void switchBlink(bool state);

	void setScreenBuffer(String screen);
	void lineToScreen(uint8_t line_to_write);
	void addLine(String line);
	void bufferToScreen();
	
	void scrollUp(bool rotate_data);
	void scrollDown(bool rotate_data);
	void scrollLeft(bool rotate_data);
	void scrollRight(bool rotate_data);
	
	void createChar(uint8_t, uint8_t[]);
	void createChar(uint8_t location, const char *charmap);

private:
	static String _empty;
	bool _check_position(bool increment);

	void init_priv();
	void send(uint8_t, uint8_t);
	void write4bits(uint8_t);
	void expanderWrite(uint8_t);
	void pulseEnable(uint8_t);
	String _screenbuffer[5]; 
	uint8_t _Addr;
	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _numlines;
	uint8_t _rows;
	uint8_t _cols;
	uint8_t _curr_row;
	uint8_t _curr_col;
	uint8_t _backlightval;
	bool _auto_scroll;

	// Unsupported API functions (not implemented in this library)
	/*
	uint8_t status();
	void setContrast(uint8_t new_val);
	uint8_t keypad();
	void setDelay(int,int);
	void on();
	void off();
	uint8_t init_bargraph(uint8_t graphtype);
	void draw_horizontal_graph(uint8_t row, uint8_t column, 
								 uint8_t len,	uint8_t pixel_col_end);
	void draw_vertical_graph(uint8_t row, uint8_t column, 
 								 uint8_t len,	uint8_t pixel_col_end);
	*/
};

#endif
