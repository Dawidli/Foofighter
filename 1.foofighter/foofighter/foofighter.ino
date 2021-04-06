// Public Library
//-------------------------
#include <Wire.h>
#include <ZumoShield.h>

//Local Library
//-------------------------
#include "Timer.h"

//Named Local Library
//-------------------------
Timer Reverse_Timer;
Timer Turn_Timer;

const int LED = 13;

// this might need to be tuned for different lighting conditions, surfaces, etc.
const int QTR_THRESHOLD = 1500; // microseconds

// these might need to be tuned for different motor types
const int REVERSE_SPEED = -400; // 0 is stopped, 400 is full speed
const int TURN_SPEED = -400;
const int FORWARD_SPEED = -400;
const int REVERSE_DURATION = 100; // ms
const int TURN_DURATION = 200; // ms

ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

const int NUM_SENSORS = 6;
unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

void waitForButtonAndCountDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);

  // play audible countdown
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);
}

void setup()
{
 //uncomment if necessary to correct motor directions
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  Serial.begin(9600);

  pinMode(LED, HIGH);

  waitForButtonAndCountDown();
}

void loop()
{
  if (button.isPressed())
  {
    // if button is pressed, stop and wait for another press to go again
    motors.setSpeeds(0, 0);
    button.waitForRelease();
    waitForButtonAndCountDown();
  }

/*
  sensors.read(sensor_values);
  Serial.print("Sensor 0 detects: ");
  Serial.println(sensor_values[0]);
  Serial.print("Sensor 5 detects: ");
  Serial.println(sensor_values[5]);
  Serial.println("");
*/
  if (sensor_values[0] > QTR_THRESHOLD)
  {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    //delay(REVERSE_DURATION);
    Reverse_Timer.getTimer(REVERSE_DURATION);
    if (Reverse_Timer.TimerHasExpired() == true)
    {
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    }
    //delay(TURN_DURATION);
    Turn_Timer.getTimer(REVERSE_DURATION);
    if (Turn_Timer.TimerHasExpired() == true)
    {
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    }
  }
  else if (sensor_values[5] > QTR_THRESHOLD)
  {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    //delay(REVERSE_DURATION);
    Reverse_Timer.getTimer(REVERSE_DURATION);
    if (Reverse_Timer.TimerHasExpired() == true)
    {
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    }
    //delay(TURN_DURATION);
    Turn_Timer.getTimer(REVERSE_DURATION);
    if (Turn_Timer.TimerHasExpired() == true)
    {
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    }
  }
  else
  {
    // otherwise, go straight
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
}
