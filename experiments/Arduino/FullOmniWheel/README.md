# Overview

The FullOmniWheel folder this README.md is found in is the primary source for the L298NDriver and the OmniWheel classes. The .h and .cpp files alongside it are copies of the files in the two folders once they have been fully debugged.

## L298NDriverTest

The folder L298NTest houses L298NTest.ino, and the copies of the L298NDriver.h and L298NDriver.cpp tested and debugged there.  Once the code of the L298N class is considered 'workable' a copy is place one level up, alongside this README.md.

## OmniWheel

The folder OmniWheel houses OmniWheel.ino, and the .h and .cpp copies of the L298NDriver and OmniWheel classes. For the L298NDriver class these should be the ones from the top folder, to avoid using a possibly buggy copy of this class. The OmniWheel.h and OmniWheel.cpp files are tested and debugged here, and when workable, are duplicated one level higher, alongside the README.md, L298NDrive.h and L298NDriver.cpp files. 

## Hardware Requirements

The OmniWheel hardware used here is an Arduino MEGA 2560, which has two L298N dual H-bridge boards connected to it at the following pins:

| WHEEL      | DIRECTION 1 | DIRECTION 2 | SPEED (PWM) |
| ---------- |:-----------:|:-----------:|:-----------:|
| FrontLeft  | 22          | 23          | 2           |
| FrontRight | 24          | 25          | 3           |
| RearLeft   | 26          | 27          | 4           |
| RearRight  | 28          | 29          | 5           |

These pins can be redefined in the main program, like it is done in the OmniWheelTest.ino file. 

## Software Requirements

These two classes require no additional libraries to function. 






