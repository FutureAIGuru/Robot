#ifndef _DRIVER_h
#define _DRIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Actuator.h"

class Driver
{
public:
    Driver(Actuator *control);
    ~Driver(void);
    
private:
    Actuator *controller;
};

#endif
