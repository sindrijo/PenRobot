#include "CustomStepper.h"


/*

1n1 = purple
1n2 = blå
1n3 = green
1n4 = yellow
== Venstre motor

1n1 = white
1n2 = grey
1n3 = purple
1n4 = blue
= høyre motor

z

*/

const int maxSpeed = 1023;
int stepsPerRev = 528;

int currentActionIndex = 0;
double currentSpeed = 0;

unsigned long time = 0;
unsigned long elapsedTime = 0;
unsigned long newActionTime = 0;

// Motor 1
int m1_p1 = 11;
int m1_p2 = 10;
int m1_p3 = 9;
int m1_p4 = 8;

// Motor 2
int m2_p1 = 7;
int m2_p2 = 6;
int m2_p3 = 5;
int m2_p4 = 4;


CustomStepper motor1 = CustomStepper(stepsPerRev, m1_p1, m1_p2, m1_p3, m1_p4);
CustomStepper motor2 = CustomStepper(stepsPerRev, m2_p1, m2_p2, m2_p3, m2_p4);

int actionDurations[6] = { 1000, 3000, 1000, 2000, 5000, 1000 };
int actionIDs[6] = { 1, 2, 1, 3, 1, 0 };
int maxActionIndex = 5;

void setup()
{
  time = millis();
  Serial.begin(9600);
  
  currentActionIndex = 0;  
  currentSpeed = 0.5;
  
  // Set first action and its duration
  setAction( actionIDs[currentActionIndex], currentSpeed );
  newActionTime += actionDurations[currentActionIndex];
  
};

void setAction(int actionId, double speedIntensity)
{
  switch( actionId )
  {
    case 1:
      Serial.println("Driving");
      drive(speedIntensity);
      break;
    case 2:
      turnLeft(speedIntensity);
      break;
    case 3:
      turnRight(speedIntensity);
      break;
    case 4:
      rotate(speedIntensity);
      break;
    case 5:
      drive(0);
      break;
    default:
      drive(1.0);
      break;
  }
}
boolean isDone = false;

void loop()
{
  elapsedTime = millis(); 
  
  /*
  Serial.print("Elapsed Time:");
  Serial.println(elapsedTime);
  
  Serial.print("Current ActionDuration:");
  int x = actionDurations[currentActionIndex];
  Serial.println(x);
  
  Serial.print("NewActionTime:");
  Serial.println(newActionTime);
  */
  

  if( elapsedTime > newActionTime && !isDone )
  {
      currentActionIndex++;
      Serial.println("Increasing action index");
      Serial.println();
      
      if( currentActionIndex > maxActionIndex )
      {
        isDone = true;
        drive(0.0);
      }
      else
      {
        newActionTime += actionDurations[currentActionIndex];
        setAction( actionIDs[currentActionIndex], currentSpeed );
      }
  }


  
  
  
  stepDaMotors();
};

void resetElapsedTime()
{
  elapsedTime = millis();
}



void stepDaMotors()
{
  Serial.println("Stepping motor.");
  motor1.step("");
  motor2.step("");
  delay(1);
}

void speed(int speed1, int speed2)
{
 if (speed1 > 1023) speed1 = 1023;
 if (speed1 < -1023) speed1 = -1023;
 if (speed2 > 1023) speed2 = 1023;
 if (speed2 < -1023) speed2 = -1023;
 
 motor1.setSpeed(speed1); 
 motor2.setSpeed(-speed2);
}
void rotate(double percent) 
{
  speed(maxSpeed * percent, -(maxSpeed * percent));
}

void turnRight(double percent)
{
  speed(maxSpeed * percent, 0);
}

void turnLeft(double percent)
{
 speed(0, maxSpeed * percent); 
}

void drive(double percent)
{
    speed(maxSpeed * percent, maxSpeed * percent); 
}



