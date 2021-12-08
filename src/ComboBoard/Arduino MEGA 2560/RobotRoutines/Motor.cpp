#include "Motor.h"

Motor::Motor(Actuator *control)
{
    controller = control;
}

Motor::~Motor(void)
{
}
