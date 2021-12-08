#include "L298NDriver.h"

L298NDriver::L298NDriver(void)
{
    _set_direction = CW;
    _set_speed = 0;
}

L298NDriver::~L298NDriver()
{
}

void L298NDriver::configurePins(int CCW_pin, int CW_pin, int speed_pin)
{
    _CCW_pin = CCW_pin;
    _CW_pin = CW_pin;
    _speed_pin = speed_pin;
    _min_speed = 0;
    _max_speed = 255;
    pinMode(_CCW_pin, OUTPUT);
    pinMode(_CW_pin, OUTPUT);
    pinMode(_speed_pin, OUTPUT);
    digitalWrite(_CCW_pin, LOW);
    digitalWrite(_CW_pin, LOW);
    analogWrite(_speed_pin, _set_speed);
}

void L298NDriver::setSpeedRange(int min_speed, int max_speed)
{
    _min_speed = min_speed;
    _max_speed = max_speed;
}

void L298NDriver::setDirection(Direction direction)
{
    setSpeed(0);
    _set_direction = direction;
    _command_motor();
}

Direction L298NDriver::getDirection()
{
    return _set_direction;
}

void L298NDriver::setSpeed(int speed)
{
    int new_speed = min(max(speed, _min_speed), _max_speed);
    while (_set_speed != speed)
    {
        _set_speed = speed;
        _command_motor();
    }
}

log_int(char *label, int value)
{
    Serial.print(label);
	  Serial.print(":");
    Serial.println(value);
}

int L298NDriver::calculateSpeed(void)
{
    return (float)(_max_speed - _min_speed)/(float)256*(float)_set_speed + _min_speed;
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
    if (_set_direction == CW)
    {
        digitalWrite(_CCW_pin, LOW);
        digitalWrite(_CW_pin, HIGH);
    }
    if (_set_direction == CCW)
    {
        digitalWrite(_CW_pin, LOW);
        digitalWrite(_CCW_pin, HIGH);
    }
    if (_set_direction == Non)
    {
        digitalWrite(_CW_pin, LOW);
        digitalWrite(_CCW_pin, LOW);
    }
}
