
#include "Movement.h"
#include <ZumoShield.h>
#include <Wire.h>

ZumoMotors motors;

Movement::Movement()
  {
    int FORWARD_SPEED = 0;
    int REVERSE_SPEED = 0;
    int TURN_SPEED = 0;
    int REVERSE_DURATION = 0;
    int TURN_DURATION = 0;
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
