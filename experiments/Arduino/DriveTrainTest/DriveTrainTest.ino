#include "L298NDriver.h"
#include "OmniWheel.h"

// pin assignments
const int left_pins[]  = {22, 23, 2};
const int right_pins[] = {24, 25, 3};

const bool separate_drivers = false;

// values for separate drivers
L298NDriver wheel_left  = L298NDriver();
L298NDriver wheel_right = L298NDriver();

bool left  = true;
bool right = true;
    
// Values for combined drive
OmniWheel drive = OmniWheel();


int spd = 0;

void setup() 
{
    Serial.begin(9600);
    if (left && separate_drivers)
    {
        wheel_left.configurePins(left_pins[0], left_pins[1], left_pins[2]);
        wheel_left.setSpeedRange(0, 255);
    }
    if (right && separate_drivers)
    {
        wheel_right.configurePins(right_pins[0], right_pins[1], right_pins[2]);
        wheel_right.setSpeedRange(0, 255);
    }
    if (!separate_drivers)
    {
        drive.configurePins(left_pins, right_pins);
    }
}

void loop() 
{
    spd += 5;
    if (spd > 255)
    {
      spd = 0;
    }
    
    if (left && separate_drivers)
    {
        wheel_left.setDirection(Left);
        wheel_left.setSpeed(spd);    
    }
    if (right && separate_drivers)
    {
        wheel_right.setDirection(Left);
        wheel_right.setSpeed(spd);    
    }
    if (!separate_drivers)
    {
        drive.setMove(Forward);
        // drive.setTurn(SpinRight);
        drive.setSpeed(spd);    
    }
    delay(100);
}
