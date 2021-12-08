#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Driver.h"

class Motor(Driver)
{
public:
    Motor(Actuator *control);
    ~Motor(void);
    
private:

};

#endif
