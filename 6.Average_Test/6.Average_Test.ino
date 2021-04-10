

const int trigPin = 11;           //connects to the trigger pin on the distance sensor
const int echoPin = 12;           //connects to the echo pin on the distance sensor
int distance = 0;               //stores the distance measured by the distance sensor
//int values[] = {2,24,9,5,19,21};

int loopCounter = 0;
const int num_of_readings = 6;
int values[num_of_readings];
int average = 0;

void setup()
{
  Serial.begin (9600);        //set up a serial connection with the computer

  pinMode(trigPin, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(echoPin, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor
}

void loop() {
  int svar = 0;
  for (int i=0; i<num_of_readings; i++)
    {
    svar = svar + values[loopCounter];
    }
  values[loopCounter] = distance;
  average = (svar / num_of_readings);
  loopCounter++;
  loopCounter%=6;
  
  distance = getDistance();   //variable to store the distance measured by the sensor

  Serial.print("Tot Sum is: ");
  Serial.print(svar);
  Serial.print("Average value is: ");
  Serial.print(average);
  Serial.print(" ,Direct value is: ");
  Serial.println(distance);
  delay(300);
  
  /*
  Serial.print(distance);     //print the distance that was measured
  Serial.println(" cm");      //print units after the distance
  */

  /*
  int svar = 0;
    for (int i=0; i<6; i++)
    {
      svar = svar + values[i];
    }
    svar = svar/6;
    Serial.println( svar );
  */
  }
  
  
//------------------FUNCTIONS-------------------------------


float getDistance()
{
  float echoTime;                   
  float calculatedDistance;        

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);      

  calculatedDistance = ((echoTime / 148.0) * 2.54);  
  return calculatedDistance;              
}
