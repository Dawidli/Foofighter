// Public Library
//-------------------------
#include <Wire.h>
#include <ZumoShield.h>

<<<<<<< HEAD
//Local Library
//-------------------------
#include "Timer.h"
#include "Movement.h"

//Named Local Library
//-------------------------
Timer Reverse_Timer;
Timer Turn_Timer;
Movement mov;

//=======================================================================

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
ZumoMotors flip;
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
  flip.flipLeftMotor(true);
  flip.flipRightMotor(true);
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

/*
  sensors.read(sensor_values);
  Serial.print("Sensor 0 detects: ");
  Serial.println(sensor_values[0]);
  Serial.print("Sensor 5 detects: ");
  Serial.println(sensor_values[5]);
  Serial.println("");
*/

  if (sensor_values[0] > QTR_THRESHOLD)                  // if leftmost sensor detects line, reverse and turn to the right
    {
    
    Reverse_Timer.getTimer(REVERSE_DURATION);            // Start timer with reverse duration
    bool revTimer = Reverse_Timer.timerHasExpired();
    
    if (revTimer == false)                               // If timer has not expired, reverse
      {
      mov.rev();
      }
    
    else if (revTimer == true)                           // If reverse timer has expired,
      {
      Turn_Timer.getTimer(TURN_DURATION);                // create new timer based on turn duration
      bool turnTimer = Turn_Timer.timerHasExpired();
        if (turnTimer == false)                          // and as long as timer has not expired, turn right
        {
        mov.turn_R();
        }
      }
    else                                                 // If all timers are expired and left sensors doesn´t detect any lines, drive forward
      {
      mov.forward();
      }
    }

                                                          // if rightmost sensor detects line, reverse and turn to the left
  else if (sensor_values[5] > QTR_THRESHOLD)
  {
    Reverse_Timer.getTimer(REVERSE_DURATION);
    bool revTimer = Reverse_Timer.timerHasExpired();        // Start timer with reverse duration
    
    if (revTimer == false)                                  // If timer has not expired, reverse
      {
      mov.rev();
      }
    else if (revTimer == true)                              // If reverse timer has expired,
      {
      Turn_Timer.getTimer(REVERSE_DURATION);                // create new timer based on turn duration
      bool turnTimer = Turn_Timer.timerHasExpired(); 
      if (turnTimer == false)                               // and as long as timer has not expired, turn right
        {
        mov.turn_L();
        }
      }
    else
    {
    mov.forward();                                          // If all timers are expired and right sensors doesn´t detect any lines, drive forward
    }
  }
}
