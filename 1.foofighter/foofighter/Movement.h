

#ifndef Movement_h
#define Movement_h
#include "Arduino.h"

class Movement
  {
  public:
    void rev_n_turn_L(bool revTimer, bool turnTimer);
    void rev_n_turn_R(bool revTimer, bool turnTimer);
    void forward();
    void rev();
    void turn_L();
    void turn_R();
    void search();
    void wait();
    void initSpeed(int forward, int reverse, int turn, int reverse_dur, int turn_dur);
    Movement();
  private:
    int FORWARD_SPEED = 0;
    int REVERSE_SPEED = 0;
    int TURN_SPEED = 0;
    int REVERSE_DURATION = 0;
    int TURN_DURATION = 0;
  };

#endif
