#ifndef _L298NDRIVER_h
#define _L298NDRIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

enum Direction { CW, CCW, Non };

class L298NDriver
{
public:
    L298NDriver(void);
    ~L298NDriver(void);
    void configurePins(int CW_pin, int CCW_pin, int speed_pin);
    void setSpeedRange(int min, int max);
    void setDirection(Direction direction);
    Direction getDirection(void);
    void setSpeed(int speed);
    int getSpeed(void);
    int calculateSpeed(void);
    void emergencyStop(void);

private:
    void _command_motor(void);

    int _CW_pin = 0;
    int _CCW_pin = 0;
    int _speed_pin = 0;
    int _set_speed = 0;
    int _min_speed = 0;
    int _max_speed = 0;
    Direction _set_direction = CW;
};


#endif
