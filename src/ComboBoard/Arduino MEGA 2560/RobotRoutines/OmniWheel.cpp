/*
	Name:		OmniWheel.cpp
	License:   	MIT
	Author:		André Slabber
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
    _fl_motor.setSpeedRange(55, 255);
    _fr_motor.setSpeedRange(40, 255);
    _rl_motor.setSpeedRange(40, 255);
    _rr_motor.setSpeedRange(40, 255);
}

void OmniWheel::setMove(Move newMove)
{
	_set_move = newMove;
    int last_speed = _set_speed;
    setSpeed(0);
	switch(_set_move)
	{
		case Forward:
			_fl_motor.setDirection(Left);
			_fr_motor.setDirection(Left);
			_rl_motor.setDirection(Left);
			_rr_motor.setDirection(Left);
			break;
		case Backward:
			_fl_motor.setDirection(Right);
			_fr_motor.setDirection(Right);
			_rl_motor.setDirection(Right);
			_rr_motor.setDirection(Right);
			break;
		case ShiftLeft:
			_fl_motor.setDirection(Right);
			_fr_motor.setDirection(Right);
			_rl_motor.setDirection(Left);
			_rr_motor.setDirection(Left);
			break;
		case ShiftRight:
			_fl_motor.setDirection(Left);
			_fr_motor.setDirection(Left);
			_rl_motor.setDirection(Right);
			_rr_motor.setDirection(Right);
			break;
	}
   setSpeed(last_speed);
}

void OmniWheel::setTurn(Turn newTurn)
{
  Serial.println("OmniWheel::setTurn(Turn newTurn)");
	_set_turn = newTurn;
  int last_speed = _set_speed;
  switch(_set_turn)
	{
		case SpinLeft:
      setSpeed(0);
			_fl_motor.setDirection(Right);
			_fr_motor.setDirection(Left);
			_rl_motor.setDirection(Right);
			_rr_motor.setDirection(Left);
	    setSpeed(last_speed);
		break;
		case SpinRight:
      setSpeed(0);
			_fl_motor.setDirection(Left);
			_fr_motor.setDirection(Right);
			_rl_motor.setDirection(Left);
			_rr_motor.setDirection(Right);
      setSpeed(last_speed);
			break;
		case TurnLeft:
			_fl_motor.setSpeed(last_speed * 0.7);
			_fr_motor.setSpeed(last_speed * 1.3);
			_rl_motor.setSpeed(last_speed * 0.7);
			_rr_motor.setSpeed(last_speed * 1.3);
			break;
		case TurnRight:
 			_fl_motor.setSpeed(last_speed * 1.3);
			_fr_motor.setSpeed(last_speed * 0.7);
			_rl_motor.setSpeed(last_speed * 1.3);
			_rr_motor.setSpeed(last_speed * 0.7);
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

void OmniWheel::setSpeed(int speed, int ramping)
{
	_set_speed = max(0, min(255, speed));
	_set_ramping = max(1, min(255, ramping));
	_fl_motor.setSpeed(_set_speed, _set_ramping);
	_fr_motor.setSpeed(_set_speed, _set_ramping);
	_rl_motor.setSpeed(_set_speed, _set_ramping);
	_rr_motor.setSpeed(_set_speed, _set_ramping);
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
