/*
	Name:		OmniWheel.cpp
	License:   	MIT
	Author:		André Slabber
*/

#include "OmniWheel.h"

OmniWheel::OmniWheel(void)
{
    _left_motor = L298NDriver();
    _right_motor = L298NDriver();
}

OmniWheel::~OmniWheel(void)
{
	emergencyStop();
}

void OmniWheel::configurePins(int left[], int right[])
{
	_left_motor.configurePins(left[0], left[1], left[2]);
	_right_motor.configurePins(right[0], right[1], right[2]);
	_left_motor.setSpeedRange(55, 255);
	_right_motor.setSpeedRange(40, 255);
}

void OmniWheel::setMove(Move newMove)
{
	_set_move = newMove;
	int last_speed = _set_speed;
	setSpeed(0);
	switch(_set_move)
	{
		case Forward:
			_left_motor.setDirection(Left);
			_right_motor.setDirection(Left);
			break;
		case Backward:
			_left_motor.setDirection(Right);
			_right_motor.setDirection(Right);
			break;
	}
	setSpeed(last_speed);
}

void OmniWheel::setTurn(Turn newTurn)
{
	_set_turn = newTurn;
	int last_speed = _set_speed;
	switch(_set_turn)
	{
		case SpinLeft:
			setSpeed(0);
			_left_motor.setDirection(Right);
			_right_motor.setDirection(Left);
			setSpeed(last_speed);
			break;
		case SpinRight:
			setSpeed(0);
			_left_motor.setDirection(Left);
			_right_motor.setDirection(Right);
			setSpeed(last_speed);
			break;
		case TurnLeft:
			_left_motor.setSpeed(last_speed * 0.7);
			_right_motor.setSpeed(last_speed * 1.3);
			break;
		case TurnRight:
 			_left_motor.setSpeed(last_speed * 1.3);
			_right_motor.setSpeed(last_speed * 0.7);
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

void OmniWheel::setSpeed(int speed)
{
	_set_speed = max(0, min(255, speed));
	_left_motor.setSpeed(_set_speed);
	_right_motor.setSpeed(_set_speed);
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
	_left_motor.emergencyStop();
	_right_motor.emergencyStop();
}
