#include "CustomStepper.h" // Made by Jens
#include <Servo.h>

/*
  Authors:
    Sindri Jóelsson
    Stefan André Brannfjell
*/


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

unsigned long elapsedTime = 0;
unsigned long newActionTime = 0;
boolean isDone = false;

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

// Initialize motors and servo
CustomStepper motor1 = CustomStepper(stepsPerRev, m1_p1, m1_p2, m1_p3, m1_p4);
CustomStepper motor2 = CustomStepper(stepsPerRev, m2_p1, m2_p2, m2_p3, m2_p4);
Servo servo;

enum act {
  E_drive,
  E_reverse,
  E_turnLeft,
  E_turnRight,
  E_rotateLeft,
  E_rotateRight,
  E_stop,
  E_startWrite,
  E_stopWrite,
};

//int actionDurations[8] = { 6350, 5000, 6350, 5000, 6350, 5000, 6350, 5000 };

typedef struct Action
{
 act action;
 int duration; 
} Action;

Action actions[] =
{
  { E_stopWrite, 300 },
  { E_startWrite, 300 },
  { E_drive, 2000 },
  { E_stopWrite, 200 },
  { E_drive, 600 },
  { E_rotateLeft, 1500 },
  { E_reverse, 300},
  { E_startWrite, 300 },
  { E_reverse, 2000 },
  { E_stopWrite, 300}  
};

// NOTE REMEMBER TO SET THIS PROPERLY, alternatively
int maxActionIndex = 10;

void setup()
{
  Serial.begin(9600);
  
  servo.attach(A0);
  
  // Set first action and its duration
  currentSpeed = 1.0
  currentActionIndex = 0;  
  setAction( actionIDs[currentActionIndex], currentSpeed );
  newActionTime += actionDurations[currentActionIndex];
  
};

void setAction(int actionId, double speedIntensity)
{
  switch( actionId )
  {
    case E_drive:
      Serial.println("Driving");
      drive(speedIntensity);
      break;
    case E_turnLeft:
      Serial.println("Turning left");
      turnLeft(speedIntensity);
      break;
    case E_turnRight:
      Serial.println("Turning right");
      turnRight(speedIntensity);
      break;
    case E_rotateLeft:
      Serial.println("Rotating left.");
      rotateLeft(speedIntensity);
      break;
    case E_rotateRight:
      Serial.println("Rotating right.");
      rotateRight(speedIntensity);
      break;
    case E_stop:
      Serial.println("STOP (Not moving)");
      drive(0);
      break;
    case E_startWrite:
      Serial.println("StartWrite (Servo)");
      drive(0);
      SetServo(90);
      break;
    case E_stopWrite:
      Serial.println("StopWrite (Servo)");
      drive(0);
      SetServo(180);
      break;
    case E_reverse:
      Serial.println("Reversing");
      drive(-speedIntensity);
      break;
    default:
      Serial.println("Default action");
      drive(0);
      break;
  }
}

void loop()
{
  elapsedTime = millis(); 
  
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
        newActionTime += actions[currentActionIndex].duration;
        setAction(
                    actions[currentActionIndex].action, 
                    currentSpeed 
                  );        
      }
  }
  
  if (!isDone)
  {
    stepDaMotors();
  }
  
};

// Servo methods
void SetServo(int angle)
{
    if( angle < 0 ) angle = 0;
    if( angle > 180 ) angle = 180;
    servo.write(angle);
}

// Stepper Methods

void stepDaMotors()
{
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

void rotateRight(double percent) 
{
    speed(maxSpeed * percent, -(maxSpeed * percent));
}
void rotateLeft(double percent) 
{
    speed(-(maxSpeed * percent), maxSpeed * percent);
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



