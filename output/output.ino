int  sensorPin = A0;
int sensorValue =0;
int percentvalue = 0;
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  sensorValue = analogRead(sensorPin);
  percentvalue = map(sensorValue,1023,200,0,100);
  Serial.print("\nsoil moisture: ");
  Serial.print(percentvalue);
  Serial.print(" ");
  delay(1000);
}
