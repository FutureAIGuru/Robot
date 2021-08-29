/*
 Name:		L298NDriver.ino
 Created:	8/21/2021 7:04:00 PM
 Author:	Moorelife
*/

// the setup function runs once when you press reset or power the board
#include "L298NDriver.h"

const int direction_pin_one = 2;
const int direction_pin_two = 4;
const int speed_pin_pwm = 3;

L298NDriver motor = L298NDriver(direction_pin_one, direction_pin_two, speed_pin_pwm);

void setup() 
{
    motor.configurePins();
}

// the loop function runs over and over again until power down or reset
void loop() 
{
    motor.setDirection(Right);
    for (int i = 100; i < 255; i++)
    {
        motor.setSpeed(i);
        delay(10);
    }
    for (int i = 255; i > 100; i--)
    {
        motor.setSpeed(i);
        delay(10);
    }
    motor.setDirection(Left);
    for (int i = 100; i < 255; i++)
    {
        motor.setSpeed(i);
        delay(10);
    }
    for (int i = 255; i > 100; i--)
    {
        motor.setSpeed(i);
        delay(10);
    }
}
