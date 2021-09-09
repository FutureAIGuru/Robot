/*
	Name:		OmniWheel.cpp
	License:   	MIT
	Author:		Moorelife
*/

#include "OmniWheel.h"

OmniWheel::OmniWheel(void)
{
    _fl_motor = L298NDriver();
    _fr_motor = L298NDriver();
    _rl_motor = L298NDriver();
    _rr_motor = L298NDriver();
}

OmniWheel::~OmniWheel(void)
{
	emergencyStop();
}

void OmniWheel::configurePins(int fl[], int fr[], int rl[], int rr[])
{
	_fl_motor.configurePins(fl[0], fl[1], fl[2]);
	_fr_motor.configurePins(fr[0], fr[1], fr[2]);
	_rl_motor.configurePins(rl[0], rl[1], rl[2]);
	_rr_motor.configurePins(rr[0], rr[1], rr[2]);
}

void OmniWheel::setMove(Move newMove)
{
	_set_move = newMove;
	switch(_set_move)
	{
		case Forward:
			_fl_motor.setDirection(Left);
			_fr_motor.setDirection(Right);
			_rl_motor.setDirection(Left);
			_rr_motor.setDirection(Right);
			break;
		case Backward:
			_fl_motor.setDirection(Right);
			_fr_motor.setDirection(Left);
			_rl_motor.setDirection(Right);
			_rr_motor.setDirection(Left);
			break;
		case Left:
			_fl_motor.setDirection(Left);
			_fr_motor.setDirection(Right);
			_rl_motor.setDirection(Right);
			_rr_motor.setDirection(Left);
			break;
		case Right:
			_fl_motor.setDirection(Right);
			_fr_motor.setDirection(Left);
			_rl_motor.setDirection(Left);
			_rr_motor.setDirection(Right);
			break;
	}
}

void OmniWheel::setTurn(Turn newTurn)
{
	_set_turn = newTurn;
	switch(_set_move)
	{
		case SpinLeft:
			_fl_motor.setDirection(Left);
			_fr_motor.setDirection(Left);
			_rl_motor.setDirection(Left);
			_rr_motor.setDirection(Left);
			break;
		case SpinRight:
			_fl_motor.setDirection(Right);
			_fr_motor.setDirection(Right);
			_rl_motor.setDirection(Right);
			_rr_motor.setDirection(Right);
			break;
		case TurnLeft:
			// TODO:
		    // needs proportional speed difference 
			// relative to current direction...
			break;
		case TurnRight:
			// TODO:
		    // needs proportional speed difference 
			// relative to current direction...
			break;
	}
}

void OmniWheel::setFaster(int steps)
{
	setSpeed(getSpeed() + steps);
}

void OmniWheel::setSlower(int steps)
{
	setSpeed(getSpeed() - steps);
}

void OmniWheel::setSpeed(int speed, int ramping = 1)
{
	_set_speed = max(0, min(255, speed));
	_set_ramping = max(0, min(255, ramping));
	_fl_motor.setSpeed(speed, ramping);
	_fr_motor.setSpeed(speed, ramping);
	_rl_motor.setSpeed(speed, ramping);
	_rr_motor.setSpeed(speed, ramping);
}

Move OmniWheel::getMove(void)
{
	return _set_move;
}

Turn OmniWheel::getTurn(void)
{
	return _set_turn;
}

int OmniWheel::getSpeed(void)
{
	return _set_speed;
}

void OmniWheel::emergencyStop(void)
{
	_fl_motor.emergencyStop();
	_fr_motor.emergencyStop();
	_rl_motor.emergencyStop();
	_rr_motor.emergencyStop();
}
