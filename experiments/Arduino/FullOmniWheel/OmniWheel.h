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

enum Move {Forward = 1, Backward = 2};
enum Turn {SpinLeft = 5, SpinRight = 6, TurnLeft = 7, TurnRight = 8};

class OmniWheel
{
public:
    OmniWheel(void);
    ~OmniWheel(void);
    void configurePins(int left[], int right[]);
    void setMove(Move nextMove);
	void setTurn(Turn newTurn);
    void setSpeed(int speed);
	void setSpeedOffset(int offset);
	void setFaster(int steps);
	void setSlower(int steps);
	Move getMove(void);
	Turn getTurn(void);
    int getSpeed(void);
    void emergencyStop(void);

private:
	Move _set_move;
	Turn _set_turn;
	int  _set_speed;
    
    L298NDriver _left_motor;
    L298NDriver _right_motor;
};

#endif
