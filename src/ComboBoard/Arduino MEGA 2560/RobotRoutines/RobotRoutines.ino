/*
	Name:		RobotRoutine.ino
	License:   	MIT
	Author:		André Slabber
*/

/*
 * Define macros for input and output pin etc.
 */
#define IR_RECEIVE_PIN      2
#define IR_SEND_PIN         3
//#define TONE_PIN            4
//#define APPLICATION_PIN     5
//#define ALTERNATIVE_IR_FEEDBACK_LED_PIN 6 // E.g. used for examples which use LED_BUILDIN for example output.
//#define IR_TIMING_TEST_PIN  7

#include <IRremote.h>

#include "L298NDriver.h"
#include "OmniWheel.h"

const int fl_pins[] = {22, 23, 2};
const int fr_pins[] = {24, 25, 3};
const int rl_pins[] = {26, 27, 4};
const int rr_pins[] = {28, 29, 5};

L298NDriver wheel_fl = L298NDriver();
L298NDriver wheel_fr = L298NDriver();
L298NDriver wheel_rl = L298NDriver();
L298NDriver wheel_rr = L298NDriver();
OmniWheel drive = OmniWheel();

int speed = 0;

// the setup function runs once when you press reset or power the board
void setup() 
{
	  Serial.begin(115200);
    drive.configurePins(fl_pins, fr_pins, rl_pins, rr_pins);
    IrReceiver.begin(8, false, false);
    Serial.print(F("Ready to receive IR signals at pin "));
    Serial.println(8);
}

void loop() 
{
    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */

    
    if (IrReceiver.decode()) 
	{
        // Print a short summary of received data
        // IrReceiver.printIRResultShort(&Serial);
        IrReceiver.resume(); // Enable receiving of the next value

        switch (IrReceiver.decodedIRData.command)
        {
            case 0x47:	Serial.println("Forward");		drive.setMove(Forward);		break;
            case 0x48:	Serial.println("Backward");		drive.setMove(Backward);	break;
            case 0x59:	Serial.println("ShiftLeft");	drive.setMove(ShiftLeft);	break;
            case 0x5A:	Serial.println("ShiftRight");	drive.setMove(ShiftRight);	break;
            case 0x34:	Serial.println("SpinRight");	drive.setTurn(SpinLeft);	break;
            case 0x35:	Serial.println("SpinLeft");		drive.setTurn(SpinRight);	break;
            case 0x33:	Serial.println("TurnLeft");	  drive.setTurn(TurnLeft);	break;
            case 0x32:	Serial.println("TurnRight");  drive.setTurn(TurnRight);	break;
            case 0x39:	Serial.println("FullStop");		drive.emergencyStop();		break;
            case 0x31:	Serial.println("Faster");		  drive.setFaster(1); 		break;
            case 0x38:	Serial.println("Slower");	  	drive.setSlower(1);	    	break;
            case 0x3B:	Serial.println("Speed 1/10");	drive.setSpeed(25);			break;
            case 0x3C:	Serial.println("Speed 2/10");	drive.setSpeed(50);			break;
            case 0x3D:	Serial.println("Speed 3/10");	drive.setSpeed(75);			break;
            case 0x3E:	Serial.println("Speed 4/10");	drive.setSpeed(100);		break;
            case 0x3F:	Serial.println("Speed 5/10");	drive.setSpeed(125);		break;
            // case 0x40:	Serial.println("Speed 6/10");	drive.setSpeed(150);		break;
            case 0x41:	Serial.println("Speed 7/10");	drive.setSpeed(175);		break;
            case 0x42:	Serial.println("Speed 8/10");	drive.setSpeed(200);		break;
            case 0x43:	Serial.println("Speed 9/10");	drive.setSpeed(225);		break;
            case 0x44:	Serial.println("Speed 10/10");	drive.setSpeed(255);		break;
        }		
	}
}
