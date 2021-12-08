#ifndef _ACTUATOR_h
#define _ACTUATOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class Actuator
{
public:
    Actuator(void);
    ~Actuator(void);
    
    void processCommand(String command_string);
    void executeCommand(void);
    void printToSerial(void);
    
    void setActNo(int value);
    void setActType(int value);
    void setPin(int value);
    void setMaxVal(int value);
    void setMinVal(int value);
    void setEnable(int value);
    void setControlMode(int value);
    void setTarget(int value);
    void setReachTime(int value);
    
    int getActNo();
    int getActType();
    int getPin();
    int getMaxVal();
    int getMinVal();
    int getEnable();
    int getControlMode();
    int getTarget();
    int getReachTime();
    
private:
    int actNo;
    int actType;
    int pin;
    int maxVal;
    int minVal;
    int enable;
    int controlMode;
    int target;
    int reachTime;
};

#endif
