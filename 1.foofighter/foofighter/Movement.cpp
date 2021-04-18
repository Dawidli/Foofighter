#include "Movement.h"
#include <ZumoShield.h>
#include <Wire.h>

ZumoMotors motors;

Movement::Movement()
  {
     FORWARD_SPEED = 0;
     REVERSE_SPEED = 0;
     TURN_SPEED = 0;
     REVERSE_DURATION = 0;
     TURN_DURATION = 0;
  }

void Movement::initSpeed(int forward, int reverse, int turn, int reverse_dur, int turn_dur)
  {
    REVERSE_SPEED = reverse;
    FORWARD_SPEED = forward;
    TURN_SPEED = turn;
    REVERSE_DURATION = reverse_dur;
    TURN_DURATION = turn_dur;
  }

void Movement::rev()
  {
    // set both motors to reverse
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
  }

void Movement::forward()
  {
    // set both motors to forward
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  
  void Movement::forward_L()
  {
    // set both motors to forward
    motors.setSpeeds(FORWARD_SPEED/4, FORWARD_SPEED);
  }
  
  void Movement::forward_R()
  {
    // set both motors to forward
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED/2);
  }

void Movement::wait()
  {
    motors.setSpeeds(0, 0);
  }

void Movement::turn_L()
  {
    motors.setSpeeds(TURN_SPEED/4, TURN_SPEED);
  }

void Movement::turn_R()
  {
    motors.setSpeeds(TURN_SPEED, TURN_SPEED/4);
  }

void Movement::search()
  {
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
  }
