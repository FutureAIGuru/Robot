// 
// 
// 

#include "L298NDriver.h"

L298NDriver::L298NDriver(void)
{
    _set_direction = Left;
    _set_speed = 0;
}

L298NDriver::~L298NDriver()
{
}

void L298NDriver::configurePins(int left_pin, int right_pin, int speed_pin)
{
    _left_pin = left_pin;
    _right_pin = right_pin;
    _speed_pin = speed_pin;
    _min_speed = 0;
    _max_speed = 255;
    pinMode(_right_pin, OUTPUT);
    pinMode(_left_pin, OUTPUT);
    pinMode(_speed_pin, OUTPUT);
    digitalWrite(_right_pin, LOW);
    digitalWrite(_left_pin, LOW);
    analogWrite(_speed_pin, _set_speed);
}

void L298NDriver::setSpeedRange(int min_speed, int max_speed)
{
    _min_speed = min_speed;
    _max_speed = max_speed;
}

void L298NDriver::setDirection(Direction direction)
{
    setSpeed(0, 10);
    _set_direction = direction;
    _command_motor();
}

Direction L298NDriver::getDirection()
{
    return _set_direction;
}

void L298NDriver::setSpeed(int speed, int ramp)
{
    int new_speed = min(max(speed, 0), 255);
    int new_ramp = min(max(ramp, 0), 255);
    int step = 0;
    if (_set_speed > speed)
    {
        step = -new_ramp;
    }
    if (_set_speed < speed)
    {
        step = new_ramp;
    }
    if (step == 0)
    {
        return;
    }
    while (_set_speed != speed)
    {
        if (abs(_set_speed - speed) < abs(step))
        {
            _set_speed = speed;
            _command_motor();
        }
        else
        {
            _set_speed += step;
            _command_motor();
        }
    }
}

int L298NDriver::calculateSpeed(void)
{
    if (_set_speed == 0)
    {
        return 0;
    }
    return _min_speed + (float)(_max_speed - _min_speed)/((float)(255))*_set_speed;
}

int L298NDriver::getSpeed(void)
{
    return _set_speed;
}

void L298NDriver::emergencyStop(void)
{
    _set_speed = 0;
    _command_motor();
}

void L298NDriver::_command_motor(void)
{
    int outspeed = calculateSpeed();
    analogWrite(_speed_pin, int(outspeed));
    if (_set_direction == Left)
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
