/*
  Name:   L2989NDriver.ino
  License:    MIT
  Author:   André Slabber
*/

/*
 * Define macros for input and output pin etc.
 */

#include "L298NDriver.h"

const int rr_pins[] = {22, 23, 2};
const int rl_pins[] = {24, 25, 3};
const int fl_pins[] = {26, 27, 4};
const int fr_pins[] = {28, 29, 5};

L298NDriver wheel_fl = L298NDriver();
L298NDriver wheel_fr = L298NDriver();
L298NDriver wheel_rl = L298NDriver();
L298NDriver wheel_rr = L298NDriver();

void setup() 
{
    int spd = 40;
    bool fl = true;
    bool fr = true;
    bool rl = true;
    bool rr = true;
    Serial.begin(115200);
    if (fl)
    {
        wheel_fl.configurePins(fl_pins[0], fl_pins[1], fl_pins[2]);
        wheel_fl.setSpeedRange(55, 255);
        wheel_fl.setDirection(Left);
        wheel_fl.setSpeed(spd);    
    }
    if (fr)
    {
        wheel_fr.configurePins(fr_pins[0], fr_pins[1], fr_pins[2]);
        wheel_fr.setSpeedRange(40, 255);
        wheel_fr.setDirection(Right);
        wheel_fr.setSpeed(spd);    
    }
    if (rl)
    {
        wheel_rl.configurePins(rl_pins[0], rl_pins[1], rl_pins[2]);
        wheel_rl.setSpeedRange(40, 255);
        wheel_rl.setDirection(Left);
        wheel_rl.setSpeed(spd);  
    }
    if (rr)
    {
        wheel_rr.configurePins(rr_pins[0], rr_pins[1], rr_pins[2]);
        wheel_rr.setSpeedRange(40, 255);
        wheel_rr.setDirection(Right);
        wheel_rr.setSpeed(spd);  
    }
}

void loop() 
{
}
