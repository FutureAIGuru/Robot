// 
// 
// 

#include "HandleActuators.h"
#include "HandleSensors.h"

Actuator* actuators[MAX_ACTUATORS];

#define ACTUATOR_UPDATE_INTERVAL 25  //update motor speed at 50 hz

const char* actuatorTypeName[] = { "motor", "servo" };


void setupActuators() {
	for (int i = 0; i < MAX_ACTUATORS; i++) {
		actuators[i] = new Actuator();
		actuators[i]->index = i;
	}
}

void handleActuators() {
	//update servo positions
	unsigned long currentTime = millis();
	for (int i = 0; i < MAX_ACTUATORS; i++)
	{
		Actuator* theActuator = actuators[i];
		int elapsedMS = currentTime - theActuator->lastMoved;
		if (theActuator->IsEnabled() && elapsedMS >= ACTUATOR_UPDATE_INTERVAL) {
			theActuator->updateActuatorValue(elapsedMS);
		}
	}
}

void Actuator::updateActuatorValue(int elapsedMS) {
}

bool Actuator::setValue(char code, int value) {
	bool handled = false;
	switch (code) {
	case 'm':
		minValue = value;
		if (targetPosition < minValue)targetPosition = minValue;
		if (currentPosition < minValue)currentPosition = minValue;
		handled = true;
		break;
	case 'M':
		maxValue = value;
		if (targetPosition > maxValue)targetPosition = maxValue;
		if (currentPosition > maxValue)currentPosition = maxValue;
		handled = true;
		break;
	case 'p':
		pinNumber = value;
		handled = true;
		break;
	case 't': //time to reach target
		targetTime = value;
		handled = true;
		break;
	case 'x':
		ActuatorType at = (ActuatorType)value;
		switch (at)
		{
		case motor:
			actuators[index] = new MotorActuator();
			break;
		case servo:
			actuators[index] = new ServoActuator(); //servo actuator?
			break;
		}
		actuators[index]->index = index;
		//Serial.print("Setting Type:"); Serial.println(value);
		actuators[index]->aType = at;
		delete this;
		handled = true;
		break;
	}
	return handled;
}

String Actuator::ToString() {
	String s = "Actuator: ";
	s = s + index + " Enb:" + boolTypeName[enabled] + " Type:" + actuatorTypeName[aType] + " curPos:" + currentPosition + " t" +
		targetTime + " p" + pinNumber + " m" + minValue + " M" + maxValue + " T" + targetPosition;
	return s;
}


/******************************************************
Special Stuff for Servos
*******************************************************/
void ServoActuator::updateActuatorValue(int elapsedMS) {
	lastMoved = millis();
	if (moving &&
		(currentPosition <= targetPosition && rate < 0 ||
			currentPosition >= targetPosition && rate > 0)) {
		currentPosition = targetPosition;
		lastMoved = millis();
		s.write((int)currentPosition);
		Serial.print("A"); Serial.print(index); Serial.print(":"); Serial.println("done");
		moving = false;
	}
	if (targetPosition != currentPosition) {
		currentPosition += (float)elapsedMS * rate;
		lastMoved = millis();
		s.write((int)currentPosition);
		Serial.print(pinNumber); Serial.print(":"); Serial.println(currentPosition);
	}
};
bool ServoActuator::setValue(char code, int value) {
	bool handled = Actuator::setValue(code, value);
	switch (code)
	{
	case 'e':
		enabled = value;
		if (!value) {
			if (s.attached())
				s.detach();
		}
		else {
			if (!s.attached()) {
				s.attach(pinNumber);
			}
		}
		handled = true;
		break;
	case 'T': //the Target position
		targetPosition = value;
		if (targetTime > 0) {
			rate = (float)(targetPosition - currentPosition) / (float)targetTime;
		}
		else {
			s.write(targetPosition);
			currentPosition = targetPosition;
			Serial.print("init servo:"); Serial.println(targetPosition);
		}
		lastMoved = millis();
		moving = true;
		handled = true;
		break;
	}
	return handled;
}

/******************************************************
Special Stuff for Motors
*******************************************************/

void MotorActuator::updateActuatorValue(int elapsedMS) {
	lastMoved = millis();
	switch (motorControlMode)
	{
	case MotorActuator::raw: //nothing here, it's all handled in the SetValue() method
		break;
	case MotorActuator::rate:
		//******************************************///
		//PID implemented here
		//PID constants are
		float KP = 0.25; float  KD = 0.05; float  KI = 0.1;

		int rateSensorPin = index + 2; //TODO fix this to read the correct sensor
		//get the current rate from the sensor
		int currentSpeed = sensorArray[rateSensorPin].currentValue;
		int currentError = currentSpeed - targetPosition;

		// input to the pid setup is the current_error
		currentError = currentSpeed - targetPosition;
		integrationSum += currentError;
		float newValue = KP * currentError + KI * integrationSum + KD * (currentError - previousError);
		newValue += 90;

		if (newValue < 0) newValue = 0;
		if (newValue > 180) newValue = 180;
		s.write((int)newValue);
		//if (index == 10) Serial.println(newValue);
		previousError = currentError;
		break;

	case MotorActuator::distance:
		break;
	}
}

bool MotorActuator::setValue(char code, int value) {
	//Serial.print("motor SetValue:"); Serial.print(code);
	//Serial.print(":"); Serial.println(value);
	bool handled = Actuator::setValue(code, value);
	switch (code)
	{
	case 'T': //the Target value
		targetPosition = value;
		//if you're in raw mode, everything happens right here...update does nothing
		if (motorControlMode == raw) {
			s.write(targetPosition);
		}
		handled = true;
		break;
	case 'e':
		enabled = value;
		if (!value) {
			if (s.attached())
				s.detach();
		}
		else {
			if (!s.attached()) {
				s.attach(pinNumber, 1000, 2000);
			}
		}
		handled = true;
		break;
	case 'c':
		MotorControlMode mcm = (MotorControlMode)value;
		motorControlMode = mcm;
		break;
	case 'd':
		distanceToTravel = value;
		handled = true;
		break;
	}
	return handled;
}


