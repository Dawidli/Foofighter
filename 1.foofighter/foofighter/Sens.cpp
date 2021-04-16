#include "Sens.h"

Sens::Sens()
{
  val = 0;
}

bool Sens::readIR(int pin, int limit)
{
  bool IR;
  val = analogRead(pin);
  //Serial.println(val);
  
  if (val >= limit)
  {
    IR = true;
  }
  else
  {
    IR = false;
  }
  return IR;
}
