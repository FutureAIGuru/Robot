#include "Actuator.h"
#include "Motor.h"

Actuator::Actuator(void)
{
    actNo = -1;
    actType = -1;
    pin = -1;
    maxVal = -1;
    minVal = -1;
    enable = -1;
    controlMode = -1;
    target = -1;
    reachTime = -1;
}

Actuator::~Actuator()
{
}

void Actuator::processCommand(String cmd_string)
{
    String cmd = cmd_string;
 	if (cmd.length() > 0)
	{
        char parm = cmd[0];
        int space = cmd.indexOf(' ');
        String parm_string = cmd.substring(1, space);
        int value = parm_string.toInt();
        Serial.print("Actuator: ");
        Serial.print(parm);
        Serial.println(value);
        
        switch (parm)
        {
            case 'A':
                setActNo(value);
                break;
            case 'x':
                setActType(value);
                break;
            case 'p':
                setPin(value);
                break;
            case 'M':
                setMaxVal(value);
                break;
            case 'm':
                setMinVal(value);
                break;
            case 'e':
                setEnable(value);
                break;
            case 'c':
                setControlMode(value);
                break;
            case 'T':
                setTarget(value);
                break;
            case 't':
                setReachTime(value);
                break;
            default:
                break;            
        }
	}    
}

void Actuator::executeCommand(void)
{
    if (getActNo() == -1) return;
    if (getPin() == -1) return;
    if (getEnable() != 1) return;
    printToSerial();
    if (getActType() == 0 &&
        getControlMode() == 0)
    {
        
    }
    
}

void Actuator::printToSerial(void)
{
    Serial.print("ActNo:");
    Serial.println(getActNo());
    Serial.print("ActType:");
    Serial.println(getActType());
    Serial.print("Pin:");
    Serial.println(getPin());
    Serial.print("MaxVal:");
    Serial.println(getMaxVal());
    Serial.print("MinVal:");
    Serial.println(getMinVal());
    Serial.print("Enable:");
    Serial.println(getEnable());
    Serial.print("ControlMode:");
    Serial.println(getControlMode());
    Serial.print("Target:");
    Serial.println(getTarget());
    Serial.print("ReachTime:");
    Serial.println(getReachTime());
}

void Actuator::setActNo(int value)
{
    actNo = value;
}

void Actuator::setTarget(int value)
{
    target = value;
}

void Actuator::setReachTime(int value)
{
    reachTime = value;
}

void Actuator::setMaxVal(int value)
{
    maxVal = value;
}

void Actuator::setMinVal(int value)
{
    minVal = value;
}

void Actuator::setPin(int value)
{
    pin = value;
}

void Actuator::setEnable(int value)
{
    enable = value;
}

void Actuator::setActType(int value)
{
    actType = value;
}

void Actuator::setControlMode(int value)
{
    controlMode = value;
}

int Actuator::getActNo()
{
    return actNo;
}

int Actuator::getActType()
{
    return actType;
}

int Actuator::getPin()
{
    return pin;
}

int Actuator::getMaxVal()
{
    return maxVal;
}

int Actuator::getMinVal()
{
    return minVal;
}

int Actuator::getEnable()
{
    return enable;
}

int Actuator::getControlMode()
{
    return controlMode;
}

int Actuator::getTarget()
{
    return target;
}

int Actuator::getReachTime()
{
    return reachTime;
}
