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
    Stop,
    RotateCW,
    RotateCCW,
    Forward,
    Backward,
    Right,
    Left,
    FrontLeft,
    RearRight,
    FrontRight,
    RearLeft,
    FrontCW,
    FrontCCW,
    RearCW,
    RearCCW,
    FrontLeftCW,
    FrontLeftCCW,
    FrontRightCW,
    FrontRightCCW
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
