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

//Name of Public Library
//-------------------------
ZumoBuzzer buzzer;
ZumoMotors flip;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

//=======================================================================
//Constants and Variables

const int LED = 13;
const int L_detect = 1000;
const int rev = 1001;
const int turn = 1002;
int revState = L_detect;
const int NUM_SENSORS = 6;
unsigned int sensor_values[NUM_SENSORS];

//Remove this int when IR_SENS get employed
int IR_SENS = 1;

// this might need to be tuned for different lighting conditions, surfaces, etc.
const int QTR_THRESHOLD = 1500; // microseconds

// these might need to be tuned for different motor types
const int REVERSE_SPEED = 400; // 0 is stopped, 400 is full speed
const int TURN_SPEED = 400;
const int FORWARD_SPEED = 200;
const int REVERSE_DURATION = 1000; // ms
const int TURN_DURATION = 2000; // ms

//========================================================================
// Functions (might remove later, not sure what class to include them in)

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
  revState = newState;
  }

void sensorValues()
  {
  Serial.print("Sensor 0 detects: ");
  Serial.println(sensor_values[0]);
  Serial.print("Sensor 5 detects: ");
  Serial.println(sensor_values[5]);
  Serial.println("");  
  }


//========================================================================

void setup()
{
  Serial.begin(9600);
  flip.flipLeftMotor(false);
  flip.flipRightMotor(false);
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

  // Checking what values sensors are detecting, printing them to Serialmonitor,
  // so that we can adjust QTR_THRESHHOLD for the enviorment 
  sensors.read(sensor_values); 
  // sensorValues();

//================================================================================
//Actual actions after initializing

  if((sensor_values[0] > QTR_THRESHOLD) or (sensor_values[5] > QTR_THRESHOLD))
    {
    rev_timer.getTimer(REVERSE_DURATION);  
    changeStateTo(rev); 
    
    switch(revState)
      {
      case rev:                                           //if the ground sensor detected something, start reversing
        mov.rev();
        if (rev_timer.timerHasExpired())
          {
          turn_timer.getTimer(TURN_DURATION);
          changeStateTo(turn);
          }
      break;
    
      case turn:                                          //when the reversing is done, start turning
        mov.turn_R();
        if (turn_timer.timerHasExpired())
          {
          changeStateTo(L_detect);
          }
      break; 
      }
    }
  else
    {
    if(IR_SENS == 2)
      {
      // attack the enemy vehicle  
      }
    else
      {
      mov.search();
      } 
    }
  }
  
