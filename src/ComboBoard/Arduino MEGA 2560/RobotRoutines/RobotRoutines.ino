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

const int fl_pins[] = {26, 27, 4};
const int fr_pins[] = {28, 29, 5};
const int rr_pins[] = {22, 23, 2};
const int rl_pins[] = {24, 25, 3};

L298NDriver wheel_fl = L298NDriver();
L298NDriver wheel_fr = L298NDriver();
L298NDriver wheel_rl = L298NDriver();
L298NDriver wheel_rr = L298NDriver();
OmniWheel drive = OmniWheel();

int speed = 0;

void ProcessCommand(String command)
{
	Serial.println(command);
	Serial3.println(command);
}

void SendInfraRedCommand(int command)
{
	switch (command)
	{
		case 0x47:	ProcessCommand("CmdIR Forward");	  break;
		case 0x48:	ProcessCommand("CmdIR Backward");	  break;
		case 0x59:	ProcessCommand("CmdIR ShiftLeft");	break;
		case 0x5A:	ProcessCommand("CmdIR ShiftRight"); break;
		case 0x34:	ProcessCommand("CmdIR SpinRight");	break;
		case 0x35:	ProcessCommand("CmdIR SpinLeft");	  break;
		case 0x33:	ProcessCommand("CmdIR TurnLeft");	  break;
		case 0x32:	ProcessCommand("CmdIR TurnRight");  break;
		case 0x39:	ProcessCommand("CmdIR FullStop");	  break;
		case 0x31:	ProcessCommand("CmdIR Faster");	  	break;
		case 0x38:	ProcessCommand("CmdIR Slower");	  	break;
		case 0x3B:	ProcessCommand("CmdIR Speed 25");	  break;
		case 0x3C:	ProcessCommand("CmdIR Speed 50");	  break;
		case 0x3D:	ProcessCommand("CmdIR Speed 75");	  break;
		case 0x3E:	ProcessCommand("CmdIR Speed 100");	break;
		case 0x3F:	ProcessCommand("CmdIR Speed 125");	break;
		case 0x40:	ProcessCommand("CmdIR Speed 150");	break;
		case 0x41:	ProcessCommand("CmdIR Speed 175");	break;
		case 0x42:	ProcessCommand("CmdIR Speed 200");	break;
		case 0x43:	ProcessCommand("CmdIR Speed 225");	break;
		case 0x44:	ProcessCommand("CmdIR Speed 255");	break;
	}
}

// the setup function runs once when you press reset or power the board
void setup() 
{
	  Serial.begin(115200);
	  Serial3.begin(115200);
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
            case 0x47:	drive.setMove(Forward);		break;
            case 0x48:	drive.setMove(Backward);	break;
            case 0x59:	drive.setMove(ShiftLeft);	break;
            case 0x5A:	drive.setMove(ShiftRight);	break;
            case 0x34:	drive.setTurn(SpinLeft);	break;
            case 0x35:	drive.setTurn(SpinRight);	break;
            case 0x33:	drive.setTurn(TurnLeft);	break;
            case 0x32:	drive.setTurn(TurnRight);	break;
            case 0x39:	drive.emergencyStop();		break;
            case 0x31:	drive.setFaster(1); 		break;
            case 0x38:	drive.setSlower(1);	    	break;
            case 0x3B:	drive.setSpeed(25);			break;
            case 0x3C:	drive.setSpeed(50);			break;
            case 0x3D:	drive.setSpeed(75);			break;
            case 0x3E:	drive.setSpeed(100);		break;
            case 0x3F:	drive.setSpeed(125);		break;
            // case 0x40:	drive.setSpeed(150);		break;
            case 0x41:	drive.setSpeed(175);		break;
            case 0x42:	drive.setSpeed(200);		break;
            case 0x43:	drive.setSpeed(225);		break;
            case 0x44:	drive.setSpeed(255);		break;
        }
		SendInfraRedCommand(IrReceiver.decodedIRData.command);
	}
}
