#include "HandlePackets.h"
#include "HandleSensors.h"
#include "HandleActuators.h"

Incoming input;

void setup() 
{
    // initialize Serial for debugging messages
    Serial.begin(115200);
    while (!Serial);
    
    // initialize Serial3 for communication with ESP8266
    Serial3.begin(115200);
    
    // initialize onboard LED and set to ON
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    
    // call the necessary initialization routines for our classes
    input.setupIncomingPackets();
    setupActuators();
    setupSensors();
    
    // signal Init complete to serial...
    Serial.println("MSG: Initialization Complete");
}

void loop() 
{
    // call the handlers we defined for the loop
    input.handleIncomingPackets();
    handleActuators();
    handleSensors();
}
