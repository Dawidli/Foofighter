int changeState(int var)
  {
  int state = 0;
  if (var == 1)
      {
      state = 3;
      }
  else
      {
      state = 1;
      }
  
  return state;
  }

void turnLedOn(int pin)
  {
  digitalWrite(pin, HIGH);  
  }

void turnLedOff(int pin)
  {
  digitalWrite(pin, LOW);  
  }




// Functions above
const int DETECT_PIN = 13;
const int ATTACK_PIN = 12;
const int SEARCH_PIN = 8;
const int DETECT_LED_PIN = 7;
const int ATTACK_LED_PIN = 4;
const int SEARCH_LED_PIN = 2;
const int detecting = 1;
const int notDetecting = 3;


int detectState = notDetecting;
int attackState = notDetecting;
int searchState = notDetecting;


  

void setup()
  {
  Serial.begin(9600);
  pinMode(DETECT_PIN, INPUT);
  pinMode(ATTACK_PIN, INPUT);
  pinMode(SEARCH_PIN, INPUT);
  pinMode(DETECT_LED_PIN, OUTPUT);
  pinMode(ATTACK_LED_PIN, OUTPUT);
  pinMode(SEARCH_LED_PIN, OUTPUT);
  }

void loop()
  {
  // if button is pressed
  bool detectStatus = digitalRead(DETECT_PIN);
  bool attackStatus = digitalRead(ATTACK_PIN);
  bool searchStatus = digitalRead(SEARCH_PIN);


  if(detectStatus)
    {
    detectState = changeState(detectState);
    delay(200);  
    }

  if(attackStatus)
      {
      attackState = changeState(attackState);
      delay(200);  
      }

   if(searchStatus)
    {
    searchState = changeState(searchState);
    delay(200);  
    }
    

    if(detectState == detecting)
      {
      attackState = notDetecting;
      searchState = notDetecting;
      turnLedOn(DETECT_LED_PIN);
      turnLedOff(ATTACK_LED_PIN);
      turnLedOff(SEARCH_LED_PIN);
      }
  
    else
      {
      turnLedOff(DETECT_LED_PIN);
      if (attackState == detecting)
        {
        searchState = notDetecting;
        turnLedOn(ATTACK_LED_PIN);
        turnLedOff(SEARCH_LED_PIN);
        }
      else
        {
        turnLedOff(ATTACK_LED_PIN);
        turnLedOn(SEARCH_LED_PIN);  
        }
      
      }
  }


  
