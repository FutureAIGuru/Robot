

#include "handlePackets.h"
#include "handleActuators.h"
#include "HandleSensors.h"


Incoming input;

//heartbeat LED
#define LED_TOGGLE_DELAY 500
unsigned long ledToggleTime = 0;
bool ledIsOn = false;

void setup() {
	Serial.begin(115200);
	Serial.println("Hello world");
	input.setupIncomingPackets();
	setupActuators();
	setupSensors();

	//setup heartbeat
	//pinMode(13, OUTPUT);
	//digitalWrite(13, HIGH);
	//ledToggleTime = millis();

	Serial.println("Initialization Complete");

}



void loop() {

	input.handleIncomingPackets();

	handleActuators();

	handleSensors();

	//if (millis() > ledToggleTime + LED_TOGGLE_DELAY) {
	//	ledToggleTime = millis();
	//	if (ledIsOn)
	//		digitalWrite(13, LOW);
	//	else
	//		digitalWrite(13, HIGH);
	//	ledIsOn = !ledIsOn;
	//}
}

