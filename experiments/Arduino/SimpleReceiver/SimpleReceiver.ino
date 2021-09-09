/*
 * SimpleReceiver.cpp
 *
 * Demonstrates receiving NEC IR codes with IRrecv
 *
 *  Copyright (C) 2020-2021  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 *
 *  MIT License
 */

/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols are active.
 */
#define RECORD_GAP_MICROS   19000
 
//#define DECODE_DENON        // Includes Sharp
//#define DECODE_JVC
//#define DECODE_KASEIKYO
//#define DECODE_PANASONIC    // the same as DECODE_KASEIKYO
//#define DECODE_LG
//#define DECODE_NEC          // Includes Apple and Onkyo
//#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_RC5
//#define DECODE_RC6

//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER

//#define DECODE_DISTANCE     // universal decoder for pulse width or pulse distance protocols
//#define DECODE_HASH         // special decoder for all protocols

#include <Arduino.h>

/*
 * Define macros for input and output pin etc.
 */
#include "PinDefinitionsAndMore.h"

#include <IRremote.h>

void setup() {
    Serial.begin(115200);
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    /*
     * Start the receiver, enable feedback LED and take LED feedback pin from the internal boards definition
     */
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);

    Serial.print(F("Ready to receive IR signals at pin "));
    Serial.println(IR_RECEIVE_PIN);
}

void loop() {
    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */
    if (IrReceiver.decode()) {

        // Print a short summary of received data
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            // We have an unknown protocol here, print more info
            // IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        else
        {
            IrReceiver.printIRResultShort(&Serial);
        }
        // Serial.println();

        /*
         * !!!Important!!! Enable receiving of the next value,
         * since receiving has stopped after the end of the current received data packet.
         */
        IrReceiver.resume(); // Enable receiving of the next value

        /*
         * Finally, check the received data and perform actions according to the received command
         */
         switch (IrReceiver.decodedIRData.command)
         {
           case 0x47:
             Serial.println("Forward");
             break;
           case 0x48:
             Serial.println("Backward");
             break;
           case 0x59:
             Serial.println("ShiftLeft");
             break;
           case 0x5A:
             Serial.println("ShiftRight");
             break;
           case 0x34:
             Serial.println("SpinRight");
             break;
           case 0x35:
             Serial.println("SpinLeft");
             break;
           case 0x33:
             Serial.println("TurnRight");
             break;
           case 0x32:
             Serial.println("TurnLeft");
             break;
           case 0x39:
             Serial.println("FullStop");
             break;
           case 0x38:
             Serial.println("Faster");
             break;
           case 0x31:
             Serial.println("Slower");
             break;
           case 0x3B:
             Serial.println("Speed 1/10");
             break;
           case 0x3C:
             Serial.println("Speed 2/10");
             break;
           case 0x3D:
             Serial.println("Speed 3/10");
             break;
           case 0x3E:
             Serial.println("Speed 4/10");
             break;
           case 0x3F:
             Serial.println("Speed 5/10");
             break;
           case 0x40:
             Serial.println("Speed 6/10");
             break;
           case 0x41:
             Serial.println("Speed 7/10");
             break;
           case 0x42:
             Serial.println("Speed 8/10");
             break;
           case 0x43:
             Serial.println("Speed 9/10");
             break;
           case 0x44:
             Serial.println("Speed 10/10");
             break;
         }
    }
}
