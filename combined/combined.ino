int  sensorPin = A0;
int sensorValue =0;
int percentvalue = 0;
int led = 13;
int sensor = 8;
int val;
void setup()
{
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(8,INPUT);
}
void loop()
{
  sensorValue = analogRead(sensorPin);
  percentvalue = map(sensorValue,1023,200,0,100);
  Serial.print("\nsoil moisture: ");
  Serial.print(percentvalue);
  Serial.print(" ");
  val = digitalRead(8);

  if(val==LOW)
  {
    digitalWrite(13,LOW);
 
  }
  else 
  {
    digitalWrite(13,HIGH);
    }
    delay(400);
  
}





  
