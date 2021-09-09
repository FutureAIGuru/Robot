/*
	Name:		L298NDriver.cpp
	License:   	MIT
	Author:		Moorelife
*/

#include "L298NDriver.h"

L298NDriver::L298NDriver(void)
{
    _left_pin = 0;
    _right_pin = 0;
    _speed_pin = 0;
    _direction = Left;
    _speed = 0;
}

L298NDriver::~L298NDriver()
{
}

void L298NDriver::configurePins(int left_pin, int right_pin, int speed_pin)
{
    _left_pin = left_pin;
    _right_pin = right_pin;
    _speed_pin = speed_pin;
    pinMode(_right_pin, OUTPUT);
    pinMode(_left_pin, OUTPUT);
    pinMode(_speed_pin, OUTPUT);
    digitalWrite(_right_pin, LOW);
    digitalWrite(_left_pin, LOW);
    analogWrite(_speed_pin, 0);
}

void L298NDriver::setDirection(Direction direction)
{
    setSpeed(0, 10);
    _direction = direction;
    _command_motor();
}

Direction L298NDriver::getDirection()
{
    return _direction;
}

void L298NDriver::setSpeed(int speed, int ramp)
{
    int new_speed = min(max(speed, 0), 255);
    int new_ramp = min(max(ramp, 0), 255);
    int step = 0;
    if (_speed > speed)
    {
        step = -new_ramp;
    }
    if (_speed < speed)
    {
        step = new_ramp;
    }
    if (step == 0)
    {
        return;
    }
    while (_speed != speed)
    {
        if (abs(_speed - speed) < abs(step))
        {
            _speed = speed;
            _command_motor();
        }
        else
        {
            _speed += step;
            _command_motor();
        }
    }
}

int L298NDriver::getSpeed(void)
{
    return _speed;
}

void L298NDriver::emergencyStop(void)
{
    _speed = 0;
    _command_motor();
}

void L298NDriver::_command_motor(void)
{
    analogWrite(_speed_pin, int(_speed));
    if (_direction == Left)
    {
        digitalWrite(_left_pin, LOW);
        digitalWrite(_right_pin, HIGH);
    }
    else
    {
        digitalWrite(_right_pin, LOW);
        digitalWrite(_left_pin, HIGH);
    }
}
