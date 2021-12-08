#include "OmniWheel.h"

int dir_cmds[][5] = 
{
   //FL     FR     RL     RR     MOVEMENT
  {CW,    CW,    CW,    CW,    Forward}, 
  {CCW,   CCW,   CCW,   CCW,   Backward}, 
  {CW,    CCW,   CCW,   CW,    Right}, 
  {CCW,   CW,    CW,    CCW,   Left}, 
  {Non,   CW,    CW,    Non,   FrontLeft}, 
  {CW,    Non,   Non,   CW,    FrontRight}, 
  {CCW,   Non,   Non,   CCW,   RearLeft}, 
  {Non,   CCW,   CCW,   Non,   RearRight}, 
  {Non,   CCW,   Non,   CCW,   FrontLeftCW}, 
  {CW,    Non,   CW,    Non,   FrontRightCW},
  {Non,   CCW,   Non,   CCW,   RearLeftCW},    // more like FrontLeftCW...
  {CW,    Non,   CW,    Non,   RearRightCW}, 
  {Non,   CW,    Non,   CW,    FrontLeftCCW},  // this might not work...
  {CCW,   Non,   CCW,   Non,   FrontRightCCW}, 
  {CCW,   Non,   CCW,   Non,   RearLeftCCW}, 
  {CW,    CW,    CW,    CW,    RearRightCCW}, 
  {Non,   Non,   CW,    CCW,   FrontCW}, 
  {Non,   Non,   CCW,   CW,    FrontCCW}, 
  {CW,    CCW,   Non,   Non,   RearCW}, 
  {CCW,   CW,    Non,   Non,   RearCCW}, 
  {CW,    CCW,   CW,    CCW,   RotateCW}, 
  {CCW,   CW,    CCW,   CW,    RotateCCW}, 
  {Non,   Non,   Non,   Non,   Stop}
};

OmniWheel::OmniWheel(void)
{
    _move = Stop;
    _speed = 0;
}

OmniWheel::~OmniWheel(void)
{
}

void OmniWheel::configurePins(int fl[], int fr[], int rl[], int rr[])
{
    _fl.configurePins(fl[0], fl[1], fl[2]);
    _fr.configurePins(fr[0], fr[1], fr[2]);
    _rl.configurePins(rl[0], rl[1], rl[2]);
    _rr.configurePins(rr[0], rr[1], rr[2]);
}

void OmniWheel::setMovement(Movement newMove)
{
    _move = newMove;
    _fl.setDirection(dir_cmds[newMove][0]);
    _fr.setDirection(dir_cmds[newMove][1]);
    _rl.setDirection(dir_cmds[newMove][2]);
    _rr.setDirection(dir_cmds[newMove][3]);
}

void OmniWheel::setSpeed(int speed)
{
	_speed = max(0, min(255, speed));
    _fl.setSpeed(_speed);
    _fr.setSpeed(_speed);
    _rl.setSpeed(_speed);
    _rr.setSpeed(_speed);
}
