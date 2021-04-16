


bool readIR(int pin, int limit)
{
  bool IR;
  int val = analogRead(pin);
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
