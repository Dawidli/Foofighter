  // Public Library
//-------------------------
#include <Wire.h>
#include <ZumoShield.h>

//Local Library
//-------------------------
#include "Movement.h"
#include "Timer.h"

//Named Local Library
//-------------------------
Movement mov;
Timer rev_timer;
Timer turn_timer;

//=======================================================================

const int LED = 13;

const int l_detect = 1000;
const int rev = 1001;
const int turn = 1002;
int state = l_detect;

// this might need to be tuned for different lighting conditions, surfaces, etc.
const int QTR_THRESHOLD = 1500; // microseconds

// these might need to be tuned for different motor types

const int REVERSE_SPEED = 400; // 0 is stopped, 400 is full speed
const int TURN_SPEED = 400;
const int FORWARD_SPEED = 200;
const int REVERSE_DURATION = 1000; // ms
const int TURN_DURATION = 2000; // ms

ZumoBuzzer buzzer;
ZumoMotors flip;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

#define NUM_SENSORS 6
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

void changeStateTo (int newState)
{
  state = newState;
}

void setup()
{
 //uncomment if necessary to correct motor directions
  flip.flipLeftMotor(false);
  flip.flipRightMotor(false);
  Serial.begin(9600);

  pinMode(LED, HIGH);

  waitForButtonAndCountDown();
}

void loop()
{

  // Initialize speeds, and puts them inside of Movement class
  mov.initSpeed(FORWARD_SPEED, REVERSE_SPEED, TURN_SPEED, REVERSE_DURATION, TURN_DURATION);
  
  // Iitial start, ( gjør om til funksjon)
  if (button.isPressed())
  {
    // if button is pressed, stop and wait for another press to go again
    mov.wait();
    button.waitForRelease();
    waitForButtonAndCountDown();
  }
  
  sensors.read(sensor_values); 
/*
  Serial.print("Sensor 0 detects: ");
  Serial.println(sensor_values[0]);
  Serial.print("Sensor 5 detects: ");
  Serial.println(sensor_values[5]);
  Serial.println("");
*/
//Serial.println(state);
switch(state)
{
  case l_detect:
  //Serial.println("state1");
    if (sensor_values[0] > QTR_THRESHOLD)                  // if leftmost sensor detects line, reverse and turn to the right
    {
    // Start two timers, which control the action inside rev_n_turn_R
    rev_timer.getTimer(REVERSE_DURATION);
    changeStateTo(rev);
    }
    else if (sensor_values[5] > QTR_THRESHOLD)                  // if leftmost sensor detects line, reverse and turn to the right
    {
    // Start two timers, which control the action inside rev_n_turn_R
    rev_timer.getTimer(REVERSE_DURATION);
    changeStateTo(rev);
    }

    else
    {
      mov.forward();
    }

    break;
    
  case rev:
  //Serial.println("state2");
    mov.rev();
    if (rev_timer.timerHasExpired())
    {
      turn_timer.getTimer(TURN_DURATION);
      changeStateTo(turn);
    }
    break;

   case turn:
   //Serial.println("state3");
     mov.turn_R();
     if (turn_timer.timerHasExpired())
    {
      changeStateTo(l_detect);
    }
    break;
    
    
}
}
  
