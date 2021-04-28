#ifndef Movement_h
#define Movement_h
#include "Arduino.h"

class Movement
  {
  public:
    void forward();
    void forward_L();
    void forward_R();
    void rev();
    void turn_L();
    void turn_R();
    void search();
    void wait();
    void initSpeed(int forward, int reverse, int turn, int reverse_dur, int turn_dur);
    Movement();
  private:
    int FORWARD_SPEED;
    int REVERSE_SPEED;
    int TURN_SPEED;
    int REVERSE_DURATION;
    int TURN_DURATION;
  };

#endif
