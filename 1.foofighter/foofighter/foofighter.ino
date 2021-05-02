// Public Library
//-------------------------
//#include <Wire.h>
#include <ZumoShield.h>

//Local Library
//-------------------------
#include "Movement.h"
#include "Timer.h"
#include "Sens.h"

//Named Local Library
//-------------------------

Movement mov;
Sens IR_R;
Sens IR_L;
Timer rev_timer;
Timer turn_timer;
Timer test_timer;

//Name of Public Library
//-------------------------
ZumoBuzzer buzzer;
ZumoMotors flip;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

//=======================================================================
//Constants and Variables

const int LED = 13;

// Contstans to the Case-system
const int rev_L = 1001;
const int rev_R = 1002;
const int turn_L = 1003;
const int turn_R = 1004;
const int forward = 1005;
const int forward_L = 1006;
const int forward_R = 1007;
const int search = 1008;
int currentState = search;

//=======================================================================
// array for the ground sensor

const int NUM_SENSORS = 6;
unsigned int sensor_values[NUM_SENSORS];

//=======================================================================
// constant variables to the IR-sensors

const int IR_L_SENS_PIN = A0;
const int IR_R_SENS_PIN = A1;
const int IRLimit = 180;

//=======================================================================
// the treshold for the floor sensors

const int QTR_THRESHOLD = 1500; // microseconds 1500 for normal bakke
//=======================================================================
//motor speed

const int REVERSE_SPEED = 300; // 0 is topped, 400 is full speed
const int TURN_SPEED = 300;
const int FORWARD_SPEED = 400;

//========================================================================
// diffrents time durations

const int REVERSE_DURATION = 1000; // ms
const int TURN_DURATION = 2000; // ms
const int TEST_DURATION = 10000; // ms


//========================================================================
// functions

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
  //test_timer.getTimer(TEST_DURATION);
}

void changeStateTo (int newState)
{
  currentState = newState;
}
// a function to test the ground sensors
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
  flip.flipLeftMotor(true);
  flip.flipRightMotor(false);
  pinMode(LED, HIGH);
  //pinMode(IR_Left, OUTPUT);
  //pinMode(IR_Right, OUTPUT);

  waitForButtonAndCountDown();

}

void loop()
{

  // Initialize speeds, and puts them inside of Movement class
  mov.initSpeed(FORWARD_SPEED, REVERSE_SPEED, TURN_SPEED, REVERSE_DURATION, TURN_DURATION);
  /*
    if(test_timer.timerHasExpired())
    {
    mov.wait();
    }
  */
  // the command for the staring frequenze
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
  //Actual actions after initializing
  //================================================================================
  //IR sens readings

  int ir_R = IR_R.readIR(IR_R_SENS_PIN, IRLimit);
  int ir_L = IR_L.readIR(IR_L_SENS_PIN, IRLimit);
// a function to test the IR sensors
  /*
    float A_0 = analogRead(IR_L_SENS_PIN);
    float A_1 = analogRead(IR_R_SENS_PIN);
    Serial.print("A0 sens value e ");
    Serial.println(A_0);
    Serial.print("A1 sens value e ");
    Serial.println(A_1);
    delay(500);
  */
  //================================================================================
  // movement controll

  //if the right-most ground sensor detects anything, it will reverse. This action can't be canceled
  if (sensor_values[5] < QTR_THRESHOLD)
  {
    rev_timer.getTimer(REVERSE_DURATION);
    changeStateTo(rev_L);
    //Serial.println("bakke detektor");
  }
  //if the left-most ground sensor detects anything, it will reverse. This action can't be canceled
  else if (sensor_values[0] < QTR_THRESHOLD)
  {
    rev_timer.getTimer(REVERSE_DURATION);
    changeStateTo(rev_R);
    //Serial.println("bakke detektor");
  }
  // if both ir-sensors detect and it is not reversing it will move straight
  else if (currentState > 1002 and ir_L and ir_R)
  {
    changeStateTo(forward);
    //Serial.println("kjøre rett fram");
  }
  // if left ir-sensor detect and it is not reversing it will move straight and to the left
  else if (currentState > 1002 and ir_L and !ir_R)
  {
    changeStateTo(forward_L);
    //Serial.println("kjøre rett fram og te venstre");
  }
  // if right ir-sensor detect and it is not reversing it will move straight and to the right
  else if (currentState > 1002 and !ir_L and ir_R)
  {
    changeStateTo(forward_R);
    //Serial.println("kjøre rett fram og te høyre");
  }
  // if none of the sensor detect anything it will go into search mode to fina the opponent
  else if (currentState > 1004 and !ir_L and !ir_R)
  {
    changeStateTo(search);
  }

  //================================================================================
  // motor controll
  Serial.println(currentState);
  switch (currentState)
  {
    // the motor command to go forward
    case forward:
      mov.forward();
      break;

    // the motor command to go forward and to the left
    case forward_L:
      mov.forward_L();
      break;

    // the motor command to go forward and to the right
    case forward_R:
      mov.forward_R();
      break;

    // the motor commnad to reverse. This cannot be canceled
    case rev_L:
      mov.rev();
      if (rev_timer.timerHasExpired())
      {
        turn_timer.getTimer(TURN_DURATION);
        changeStateTo(turn_L);
      }
      break;

    // the motor commnad to reverse. This cannot be canceled
    case rev_R:
      mov.rev();
      if (rev_timer.timerHasExpired())
      {
        turn_timer.getTimer(TURN_DURATION);
        changeStateTo(turn_R);
      }
      break;

    // the motor command to swing to the right. This commando can be canceld if the IR-sens detect anything
    case turn_R:
      mov.turn_R();
      if (turn_timer.timerHasExpired())
      {
        changeStateTo(search);
      }
      break;

    // the motor command to swing to the left. This commando can be canceld if the IR-sens detect anything
    case turn_L:
      mov.turn_L();
      if (turn_timer.timerHasExpired())
      {
        changeStateTo(search);
      }
      break;

    // the motor command to search fot the opponent
    case search:
      mov.search(); 
      break;
  }
}
