/*
	Name:		OmniWheel.h
	License:   	MIT
	Author:		Moorelife
*/

#ifndef _OMNIWHEEL_h
#define _OMNIWHEEL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "L298NDriver.h"

enum Move {Forward = 1, Backward = 2, ShiftLeft = 3, ShiftRight = 4};
enum Turn {SpinLeft = 5, SpinRight = 6, TurnLeft = 7, TurnRight = 8};

class OmniWheel
{
public:
    OmniWheel(void);
    ~OmniWheel(void);
    void configurePins(int fl[], int fr[], int rl[], int rr[]);
    void setMove(Move nextMove);
	void setTurn(Turn newTurn);
    void setSpeed(int speed, int ramping = 1);
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
	int  _set_ramping;
    
    L298NDriver _fl_motor;
    L298NDriver _fr_motor;
    L298NDriver _rl_motor;
    L298NDriver _rr_motor;
};

#endif
