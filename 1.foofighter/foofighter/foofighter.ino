 // Public Library
//-------------------------
#include <Wire.h>
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//=====================================================================
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


// change this to make the song slower or faster
int tempo = 140;

// change this to whichever pin you want to use
const int buzzer_s = 3;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;
//=======================================================================


//const int LEDPIN = 13;
//const int BTNPIN = 12;
const int duration = 50;
const int ledOn = 2000;
const int ledOff = 2001;
const int songOn = 3000;
const int songOff = 3001;
int songState = songOff;
int ledState = ledOn;
int btnCount = 0;

Timer ledTime;
Timer songTime;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const int LED = 13;


const int rev_L = 1001;
const int rev_R = 1002;
const int turn_L = 1003;
const int turn_R = 1004;
const int forward = 1005;
const int search = 1006;

int currentState = search;


const int NUM_SENSORS = 6;
unsigned int sensor_values[NUM_SENSORS];

const int IR_SENS_PIN = A0;
const int IRLimit = 200;



// the treshold for the floor sensors
// this might need to be tuned for different lighting conditions, surfaces, etc.
const int QTR_THRESHOLD = 1500; // microseconds

//motor speed
// these might need to be tuned for different motor types
const int REVERSE_SPEED = 200; // 0 is stopped, 400 is full speed
const int TURN_SPEED = 200;
const int FORWARD_SPEED = 200;
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
  waitForButtonAndCountDown();
}

void loop()
  { 
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer_s, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    songTime.getTimer(noteDuration);
    while(!songTime.timerHasExpired())
      {}
    
    //delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer_s); 
  
  // Initialize speeds, and puts them inside of Movement class
  mov.initSpeed(FORWARD_SPEED, REVERSE_SPEED, TURN_SPEED, REVERSE_DURATION, TURN_DURATION);
  /*

  if(test_timer.timerHasExpired())
    {
    mov.wait();  
    }*/
  if (button.isPressed())
    {
    // if button is pressed, stop and wait for another press to go again
    mov.wait();
    button.waitForRelease();
    waitForButtonAndCountDown();
    test_timer.getTimer(TEST_DURATION);
    }

  // Checking what values sensors are detecting, printing them to Serialmonitor,
  // so that we can adjust QTR_THRESHHOLD for the enviorment 
  sensors.read(sensor_values); 
  // sensorValues();

//================================================================================
//Actual actions after initializing

  int ir = IR_R.readIR(IR_SENS_PIN, IRLimit); // Infra red sensor reading

//================================================================================
// movement controll
  if(sensor_values[5] > QTR_THRESHOLD) 
    {
    rev_timer.getTimer(REVERSE_DURATION);
    changeStateTo(rev_R);
    }
  else if(sensor_values[0] > QTR_THRESHOLD)
    {
    rev_timer.getTimer(REVERSE_DURATION);
    changeStateTo(rev_L); 
    }
  
  else if (currentState > 1002 and ir)
    {
    changeStateTo(forward);
    }  
  else if (currentState > 1004 and !ir)
    {
    changeStateTo(search);
    }
//================================================================================
// motor controll
   
  switch(currentState)
    {
    case forward:
      mov.forward();
      break;

    case rev_L:
      mov.rev();
      if(rev_timer.timerHasExpired())
        {
        turn_timer.getTimer(TURN_DURATION);
        changeStateTo(turn_R); 
        }
      break;

    case rev_R:
      mov.rev();
      if(rev_timer.timerHasExpired())
        {
        turn_timer.getTimer(TURN_DURATION);
        changeStateTo(turn_L);  
        }
      break;

    case turn_R:
      mov.turn_R();
      if(turn_timer.timerHasExpired())
        {
        changeStateTo(search);
          
        }
      break;

    case turn_L:
      mov.turn_L();
      if(turn_timer.timerHasExpired())
        {
        changeStateTo(search);  
        
        }

        break;

     case search:
     mov.wait();
     break;
    }
  
  }
    
}
