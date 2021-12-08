#include "L298NDriver.h"

// pin assignments
const int pins_fl[] = {22, 23, 2};
const int pins_fr[] = {24, 25, 3};
const int pins_rl[] = {26, 27, 4};
const int pins_rr[] = {28, 29, 5};

// values for separate drivers
L298NDriver wheel_fl = L298NDriver();
L298NDriver wheel_fr = L298NDriver();
L298NDriver wheel_rl = L298NDriver();
L298NDriver wheel_rr = L298NDriver();

bool fl = true;
bool fr = true;
bool rl = true;
bool rr = true;
    
int spd = 0;
Direction dir = CW;

void setup() 
{
    Serial.begin(115200);
    if (fl)
    {
        wheel_fl.configurePins(pins_fl[0], pins_fl[1], pins_fl[2]);
        wheel_fl.setSpeedRange(20, 255);
    }
    if (fr)
    {
        wheel_fr.configurePins(pins_fr[0], pins_fr[1], pins_fr[2]);
        wheel_fr.setSpeedRange(20, 255);
    }
    if (rl)
    {
        wheel_rl.configurePins(pins_rl[0], pins_rl[1], pins_rl[2]);
        wheel_rl.setSpeedRange(30, 255);
    }
    if (rr)
    {
        wheel_rr.configurePins(pins_rr[0], pins_rr[1], pins_rr[2]);
        wheel_rr.setSpeedRange(20, 255);
    }
}

void loop() 
{
    spd += 5;
    if (spd > 255)
    {
      spd = 0;
      if (dir == CW)
      {
        dir = CCW;
      }
      else
      {
        dir = CW;
      }
    }
    
    if (fl)
    {
        wheel_fl.setDirection(dir);
        wheel_fl.setSpeed(spd);    
    }
    if (fr)
    {
        wheel_fr.setDirection(dir);
        wheel_fr.setSpeed(spd);    
    }
    if (rl)
    {
        wheel_rl.setDirection(dir);
        wheel_rl.setSpeed(spd);    
    }
    if (rr)
    {
        wheel_rr.setDirection(dir);
        wheel_rr.setSpeed(spd);    
    }
    delay(100);
}
