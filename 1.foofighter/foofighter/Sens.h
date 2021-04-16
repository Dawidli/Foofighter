#ifndef Sens_h
#define Sens_h
#include "Arduino.h"

class Sens
{
  public:
    bool readIR(int pin, int limit);
    Sens();
  
  private:
    int val;
};


#endif
