

const int trigPin = 11;           //connects to the trigger pin on the distance sensor
const int echoPin = 12;           //connects to the echo pin on the distance sensor
float distance = 0;

int tall[] = {2,24,9,5,19,21};
int num_tall = 7;
int tot = 0; 

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

  //int tall[] = {2,24,9,5,19,21};
  //int num_tall = 6;
  //int loopCounter = 0;
  //int sens_val[6]= {1,1,1,1,1,1}
  
  float sens_val[num_tall];
  
  
    sens_val[loopCounter] = distance;
    loopCounter++;
    loopCounter%=num_tall;
    
    float total = 0;
    for (int i=0; i<num_tall; i++)
      {
      total = total + sens_val[i];  
      }
    

    float testTall = sens_val[0] + sens_val[1] + sens_val[2] + sens_val[3] + sens_val[4] + sens_val[5] + sens_val[6];
    float avg = total/num_tall;



  Serial.print("  tall 0: ");
  Serial.println(sens_val[0]);
  Serial.print("  tall 1: ");
  Serial.println(sens_val[1]);
  Serial.print("  tall 2: ");
  Serial.println(sens_val[2]);
  Serial.print("  tall 3: ");
  Serial.println(sens_val[3]);
  Serial.print("  tall 4: ");
  Serial.println(sens_val[4]);
  Serial.print("  tall 5: ");
  Serial.println(sens_val[5]);
  Serial.print("  tall 6: ");
  Serial.println(sens_val[6]);
  Serial.print("     AVG: ");
  Serial.println(avg);
  Serial.print("     RAW: ");
  Serial.println(distance);
  Serial.println("---------------");
  
  delay(500);

  distance = getDistance();
  
  /*
  int svar = 0;
  for (int i=0; i<num_of_readings; i++)
    {
    svar = svar + values[loopCounter];
    loopCounter++;
    loopCounter%=6;
    }
  values[loopCounter] = distance;
  average = (svar / num_of_readings);
  
  
  distance = getDistance();   //variable to store the distance measured by the sensor

  Serial.print("Tot Sum is: ");
  Serial.print(svar);
  Serial.print(", Average value is: ");
  Serial.print(average);
  Serial.print(" ,Direct value is: ");
  Serial.println(distance);
  delay(300);
  */
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
