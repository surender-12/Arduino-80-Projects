#include <Servo.h>

const int trigPin = 10;
const int echoPin = 11;
long duration;
int distance;
Servo myServo;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  myServo.attach(12);
}

void loop() {
  // Move to 180° instantly (servo moves at max speed)
  myServo.write(180);
  delay(200);  // wait for servo to reach (adjust based on servo spec)
  distance = calculateDistance();
  Serial.print("180,"); Serial.println(distance);

  // Move back to 0° instantly
  myServo.write(0);
  delay(200);  // wait for servo to reach
  distance = calculateDistance();
  Serial.print("0,"); Serial.println(distance);
}

int calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // in cm
  return distance;
}
