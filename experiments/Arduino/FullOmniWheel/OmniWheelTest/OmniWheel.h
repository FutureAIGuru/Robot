/*
	Name:		OmniWheel.h
	License:   	MIT
	Author:		André Slabber
*/

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
	Forward = 0,      	Backward = 1,      	Left = 2,        	Right = 3,
	FrontLeft = 4,    	FrontRight = 5,    	RearLeft = 6,    	RearRight = 7,
	FrontLeftCW = 8,  	FrontRightCW = 9,  	RearLeftCW = 10, 	RearRightCW = 11, 
	FrontLeftCCW = 12, 	FrontRightCCW = 13, RearLeftCCW = 14, 	RearRightCCW = 15, 
	FrontCW = 16,       FrontCCW = 17,      RearCW = 18,        RearCCW = 19, 
	RotateCW = 20,      RotateCCW = 21, 	Stop = 22
};

int directions[][] = 
{
	//FL     FR     RL     RR    MOVEMENT
	{Left,  Left,  Left,  Left,  Forward}, 
	{Right, Right, Right, Right, Backward}, 
	{Left,  Right  Right, Left,  Left}, 
	{Right, Left,  Left,  Right  Right}, 
	{None,  Left,  Left,  None,  FrontLeft}, 
	{Left,  None,  None,  Left,  FrontRight}, 
	{Right, None,  None,  Right, RearLeft}, 
	{None,  Right, Right, None,  RearRight}, 
	{None,  Right, None,  Right, FrontLeftCW}, 
	{Left,  None,  Left,  None,  FrontRightCW},  // this might not work...
	{None,  Right, None,  Right, RearLeftCW},    // this might not work...
	{Left,  None,  Left,  None,  RearRightCW}, 
	{None,  Left,  None,  Left,  FrontLeftCCW},  // this might not work...
	{Right, None,  Right, None,  FrontRightCCW}, 
	{Right, None,  Right, None,  RearLeftCCW}, 
	{Left,  Left,  Left,  Left,  RearRightCCW}, 
	{Left,  Left,  Left,  Left,  FrontCW}, 
	{Left,  Left,  Left,  Left,  FrontCCW}, 
	{Left,  Left,  Left,  Left,  RearCW}, 
	{Left,  Left,  Left,  Left,  RearCCW}, 
	{Left,  Right, Left,  Right, RotateCW}, 
	{Right, Left,  Right, Left,  RotateCCW}, 
	{None,  None,  None,  None,  Stop}
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
