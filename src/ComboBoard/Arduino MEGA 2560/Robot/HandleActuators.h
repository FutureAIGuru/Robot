// HandleServos.h

#ifndef _HandleActuators_h
#define _HandleActuators_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <Servo.h>

#define MAX_ACTUATORS 20

enum ActuatorType {motor,servo};
class Actuator {
public:
	int index=-1;
	ActuatorType aType = servo;     //'x'
	float currentPosition = 90;	//degrees
	int targetPosition = 90;		//'T' degrees
	int targetTime = 4000;			//'t'

	int sensor = -1;				//associated sensor (if any)
	float rate = 0;				    // steps/milli

	Servo s;			//pwm driver

	bool IsEnabled() { return enabled; };

	unsigned long lastMoved = millis();
	
protected:
	bool enabled = false;			//'e' enabled / attached
	int pinNumber = -1;			//'p'
	int minValue = 0;		//'m' don't go below this value
	int maxValue = 180;		//'M' don't go above this value

public:
	virtual void updateActuatorValue(int elapsedMS);
	virtual bool setValue(char code, int value);
	String ToString();
};

class ServoActuator : public Actuator {
public:
	bool moving = false;
	virtual void updateActuatorValue(int elapsedMS);
	bool setValue(char code, int value);
};

class MotorActuator : public Actuator {
public:
	enum MotorControlMode {raw,rate,distance};
	MotorControlMode motorControlMode = rate;

	float integrationSum = 0; //for PID controller
	float previousError = 0;
	int distanceToTravel = 0; //used for motors only
	int rateSensorPin = -1;

	void updateActuatorValue(int elapsedMS);
	bool setValue(char code, int value);
};

extern Actuator * actuators[MAX_ACTUATORS];

void setupActuators();

void handleActuators();

#endif
