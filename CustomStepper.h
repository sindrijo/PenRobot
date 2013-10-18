// CustomStepper.h
// Allows bidirectional operation of a 28BYJ-48, using a ULN2003
// interface board to drive the stepper. The 28BYJ-48 motor is a
// 4-phase, 8-beat motor, geared down by a factor of 68. One bipolar
// winding is on motor pins 1 & 3 and the other on motor pins 2 & 4.
//
// The step angle is 5.625/64 and the operating Frequency is 100pps.
// Current draw is 92mA.
////////////////////////////////////////////////

#ifndef _CUSTOMSTEPPER_H
#define _CUSTOMSTEPPER_H

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

// Motor magnet setup for Half-stepping
static const uint8_t
	lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};

class CustomStepper {
	public:
		CustomStepper( uint16_t stepsPerRevolution, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4 );
		uint16_t step( char *name );
		void setSpeed( int16_t speed );
		void wait();
	protected:
		void clockwise( int16_t count );
		void anticlockwise( int16_t count );
		void setOutput(int8_t out);
		
		// Motor pins
		uint8_t motorPin1;    // Blue   - 28BYJ48 pin 1
		uint8_t motorPin2;    // Pink   - 28BYJ48 pin 2
		uint8_t motorPin3;    // Yellow - 28BYJ48 pin 3
		uint8_t motorPin4;    // Orange - 28BYJ48 pin 4
		
		// Steps per full revolution
		int16_t countsperrev;
		
		// The current step
		int16_t count;
		int16_t constantCount;
		
		// Stepper minimum delay between writes (max speed)
		uint16_t motorSpeedMin;
		uint16_t motorSpeedMax;
		
		// The user requested speed, set with setSpeed()
		int16_t userSpeed;
		
		// Calculated speed based on what was set with setSpeed()
		int16_t stepEveryXFrames;
};

#endif // _CUSTOMSTEPPER_H