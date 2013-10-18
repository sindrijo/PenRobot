// CustomStepper.cpp
#include "CustomStepper.h"

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

CustomStepper::CustomStepper( uint16_t stepsPerRevolution, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) {
	Serial.begin( 9600 );
	// set defaults
	count = 0;
	constantCount = 0;
	
	motorSpeedMin = 1200;	 // Minimum delay between writes ( = max speed)
	motorSpeedMax = 16000; // The largest value for delayMicroseconds for an accurate delay is 16383
	stepEveryXFrames = 1023;
	
	// save the input values
	countsperrev = stepsPerRevolution;
	motorPin1 = pin1;
	motorPin2 = pin2;
	motorPin3 = pin3;
	motorPin4 = pin4;
	
  //declare the motor pins as outputs
  pinMode( motorPin1, OUTPUT );
  pinMode( motorPin2, OUTPUT );
  pinMode( motorPin3, OUTPUT );
  pinMode( motorPin4, OUTPUT );
}

//////////////////////////////////////////////////////////////////////////////
void CustomStepper::setSpeed( int16_t speed ) {
	// safeguard the input!
	userSpeed = constrain(speed, -1023, 1023);
	
	// ensure we're positive about things
	float tempSpeed = userSpeed;
	if( tempSpeed < 0 ){
		tempSpeed *= -1;
	}
	
	// size up and 
	stepEveryXFrames = 1024 / tempSpeed; // map( tempSpeed, 0, 1023, motorSpeedMax, motorSpeedMin );
	/*
	Serial.print(" speed: ");
	Serial.print(speed);
	Serial.print(" tempSpeed: ");
	Serial.print(tempSpeed);
	Serial.print(" stepEveryXFrames: ");
	Serial.println(stepEveryXFrames);
	*/
}

//////////////////////////////////////////////////////////////////////////////
uint16_t CustomStepper::step( char *name ) {
	
	constantCount++;
	int test = constantCount % stepEveryXFrames;
	if( test == 0){
 	 	if(userSpeed > 0 ){
		    clockwise( count );
				count++;
		  } else if (userSpeed < 0) {
		    anticlockwise( count );
				count--;
		  } else {
		    // stand still
			}

			// make sure the count stays within bounds
			if (count > countsperrev){
			    count = 0;
			} else if (count < 0){
					count = countsperrev;
			}
	}
	
	return count;
}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)
void CustomStepper::anticlockwise( int16_t count ) {
  int num = count % 8;
	//Serial.print( "anticlockwise" );
  setOutput( num );
}

void CustomStepper::clockwise( int16_t count ) {
  int num = (count % 8);
	//Serial.print( "clockwise" );
  setOutput( num );
}

void CustomStepper::wait() {
  delayMicroseconds( motorSpeedMin );
}

void CustomStepper::setOutput(int8_t out) {
	//Serial.println( out );
  digitalWrite(motorPin1, bitRead(lookup[out], 0));
  digitalWrite(motorPin2, bitRead(lookup[out], 1));
  digitalWrite(motorPin3, bitRead(lookup[out], 2));
  digitalWrite(motorPin4, bitRead(lookup[out], 3));
	//delay(250);
}
