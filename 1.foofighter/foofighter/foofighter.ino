// Public Library import
//=======================================================================
#include <ZumoShield.h>

//Local Library import
//=======================================================================
#include "Movement.h"
#include "Timer.h"
#include "Sens.h"

//Named Local Library
//=======================================================================
Movement mov;
Sens IR_R;
Sens IR_L;
Timer rev_timer;
Timer turn_timer;
Timer test_timer;

//Name of Public Library
//=======================================================================
ZumoBuzzer buzzer;
ZumoMotors flip;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

// array AND defenition for the ground sensor
//=======================================================================

const int NUM_SENSORS = 6;
unsigned int sensor_values[NUM_SENSORS];
int groundSens_L = sensor_values[5];
int groundSens_R = sensor_values[0];

// the treshold for the floor sensors
//=======================================================================

const int QTR_THRESHOLD = 1500; // microseconds 1500

//Variables for the program
//=======================================================================

const int LED = 13; //for the ligthning

// Constants to the Case-system
//=======================================================================
const int REVERSE_L = 1001; 
const int REVERSE_R = 1002; 
const int TURN_L = 1003;
const int TURN_R = 1004;
const int OPPONENT_F = 1005; //F stands for forward
const int OPPONENT_L = 1006; //L stands for left
const int OPPONENT_R = 1007; //R stands for right
const int SEARCH = 1008;
int currentState = SEARCH;

//Constant variables to the Infrared sensors atop of the robot
//=======================================================================

const int IR_L_SENS_PIN = A0;
const int IR_R_SENS_PIN = A1;
const int IRLIMIT = 180; //Distance value
int ir_R = 0;
int ir_L = 0;

//Detection states for the sensor system
//=======================================================================
bool groundSens_R_active = (groundSens_R < QTR_THRESHOLD);
bool groundSens_L_active = (groundSens_L < QTR_THRESHOLD);
bool IR_R_active = !ir_L and ir_R;
bool IR_L_active = ir_L and !ir_R;
bool both_ir_active = ir_L and ir_R;
bool both_ir_unactive = !ir_L and !ir_R;
bool not_reversing = !(REVERSE_L or REVERSE_R);

//motor speed
//=======================================================================

const int REVERSE_SPEED = 300; // 0 is not moving, 400 is full speed
const int TURN_SPEED = 300;
const int FORWARD_SPEED = 400;

// different time durations
//========================================================================

const int REVERSE_DURATION = 1000; // ms
const int TURN_DURATION = 2000; // ms

// functions
//========================================================================

// Function that is used to start the robot and play a countdown for 5 seconds
// this wait period is required for the competition
void waitForButtonAndCountDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);

  // play audible countdown, this sequence last for 5 seconds
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);
  //test_timer.getTimer(TEST_DURATION);
}

// A function to change the global variable currentState, which is used to
// to determine what action robot is about to do
void changeStateTo (int newState)
{
  currentState = newState;
}

//========================================================================

void setup()
{
  // Replace true and false if the motors are spinning in the wrong direction
  flip.flipLeftMotor(true);
  flip.flipRightMotor(false);

  // Turn on led
  pinMode(LED, HIGH);

  // Wait for buttonpress to start a countdown and end setup sequence after countdown
  waitForButtonAndCountDown();

}

void loop()
{

  // Initialize speeds, and puts them inside of Movement class
  mov.initSpeed(FORWARD_SPEED, REVERSE_SPEED, TURN_SPEED, REVERSE_DURATION, TURN_DURATION);

  // the command for the starting sequence
  if (button.isPressed())
  {
    // if button is pressed, stop and wait for another press to go again
    mov.wait();
    button.waitForRelease();
    waitForButtonAndCountDown();
  }
  
  //Actual actions after initializing

  //IR sens readings
  //================================================================================
  

  ir_R = IR_R.readIR(IR_R_SENS_PIN, IRLIMIT);
  ir_L = IR_L.readIR(IR_L_SENS_PIN, IRLIMIT);

  // movement controll
  //================================================================================

  //if the right-most ground sensor detects anything, it will reverse. This action can't be canceled
  if (groundSens_L_active)
  {
    rev_timer.getTimer(REVERSE_DURATION);
    changeStateTo(REVERSE_L);
  }
  //if the left-most ground sensor detects anything, it will reverse. This action can't be canceled
  else if (groundSens_R_active)
  {
    rev_timer.getTimer(REVERSE_DURATION);
    changeStateTo(REVERSE_R);
  }
  //if the robot is not reversing, it will start looking after the opponent
  else if(not_reversing)
  {
     // if both ir-sensors detect and it is not reversing it will move straight
    if (both_ir_active)
    {
      changeStateTo(OPPONENT_F);
    }
    // if left ir-sensor detect and it is not reversing it will move straight and to the left
    else if (IR_L_active)
    {
      changeStateTo(OPPONENT_L);
    }
    // if right ir-sensor detect and it is not reversing it will move straight and to the right
    else if (IR_R_active)
    {
    changeStateTo(OPPONENT_R);
    }
    // if none of the sensor detect anything it will go into search mode to find the opponent
    else if (both_ir_unactive)
    {
      changeStateTo(SEARCH);
    }
  }
  
  // motor controll
  //================================================================================
  switch (currentState)
  {
    // the motor command to go forward
    case OPPONENT_F:
      mov.forward();
      break;

    // the motor command to go forward and to the left
    case OPPONENT_L:
      mov.forward_L();
      break;

    // the motor command to go forward and to the right
    case OPPONENT_R:
      mov.forward_R();
    break;

    // the motor commnad to reverse. This cannot be canceled
    case REVERSE_L:
      mov.rev();
      if (rev_timer.timerHasExpired())
      {
        turn_timer.getTimer(TURN_DURATION);
        changeStateTo(TURN_L);
      }
      break;

    // the motor commnad to reverse. This cannot be canceled
    case REVERSE_R:
      mov.rev();
      if (rev_timer.timerHasExpired())
      {
        turn_timer.getTimer(TURN_DURATION);
        changeStateTo(TURN_R);
      }
      break;

    // the motor command to turn to the right. This commando can be canceled if the IR-sens detects anything
    case TURN_R:
      mov.turn_R();
      if (turn_timer.timerHasExpired())
      {
        changeStateTo(SEARCH);
      }
      break;

    // the motor command to turn to the left. This commando can be canceled if the IR-sens detects anything
    case TURN_L:
      mov.turn_L();
      if (turn_timer.timerHasExpired())
      {
        changeStateTo(SEARCH);
      }
      break;

    // the motor command to search for the opponent
    case SEARCH:
      mov.search(); 
      break;
  }
}
