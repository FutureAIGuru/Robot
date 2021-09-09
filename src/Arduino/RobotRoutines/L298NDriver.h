/*
	Name:		L298NDriver.h
	License:   	MIT
	Author:		Moorelife
*/

#ifndef _L298NDRIVER_h
#define _L298NDRIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

enum Direction {Left = 9, Right = 10};

class L298NDriver
{
public:
    L298NDriver(void);
    ~L298NDriver(void);
    void configurePins(int left_pin, int right_pin, int speed_pin);
    void setDirection(Direction direction);
    void setSpeed(int speed, int ramping = 1);
    int getSpeed(void);
    Direction getDirection(void);
    void emergencyStop(void);

private:
    void _command_motor(void);

    int _left_pin = 0;
    int _right_pin = 0;
    int _speed_pin = 0;
    int _speed = 0;
    Direction _direction = Left;
};


#endif
