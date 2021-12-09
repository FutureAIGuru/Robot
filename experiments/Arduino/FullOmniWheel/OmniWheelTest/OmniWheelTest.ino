
#include "OmniWheel.h"

// drive motor pin assignments
const int pins_fl[] = {22, 23, 2};
const int pins_fr[] = {24, 25, 3};
const int pins_rl[] = {26, 27, 4};
const int pins_rr[] = {28, 29, 5};

OmniWheel platform;
    
int spd = 0;
int move = 0;

void setup() 
{
    Serial.begin(115200);
    platform.configurePins(pins_fl, pins_fr, pins_rl, pins_rr);    
}

void loop() 
{
    for (move = 0; move < 19; move++)
    {
        platform.setMovement((Movement)move);
        for (spd = 0; spd < 255; spd++)
        {
            platform.setSpeed(spd);
            delay(10);
        }
    }
}
