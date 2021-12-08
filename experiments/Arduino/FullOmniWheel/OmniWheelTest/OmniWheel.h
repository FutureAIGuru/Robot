#ifndef _OMNIWHEEL_h
#define _OMNIWHEEL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "L298NDriver.h"

enum Movement 
{	
	Forward = 0,      	Backward = 1,      	Right = 2,        	Left = 3,
	FrontLeft = 4,    	FrontRight = 5,    	RearLeft = 6,    	  RearRight = 7,
	FrontLeftCW = 8,  	FrontRightCW = 9,  	RearLeftCW = 10, 	  RearRightCW = 11, 
	FrontLeftCCW = 12, 	FrontRightCCW = 13, RearLeftCCW = 14, 	RearRightCCW = 15, 
	FrontCW = 16,       FrontCCW = 17,      RearCW = 18,        RearCCW = 19, 
	RotateCW = 20,      RotateCCW = 21,   	Stop = 22
};


class OmniWheel
{
public:
    OmniWheel(void);
    ~OmniWheel(void);
    void configurePins(int fl[], int fr[], int rl[], int rr[]);
    void setMovement(Movement nextMove);
    void setSpeed(int speed);
	void setFaster(int steps);
	void setSlower(int steps);
	Movement getMovement(void);
    int getSpeed(void);
    void emergencyStop(void);

private:
	Movement _move;
	int      _speed;
    
    L298NDriver _fl;
    L298NDriver _fr;
    L298NDriver _rl;
    L298NDriver _rr;
};

#endif
