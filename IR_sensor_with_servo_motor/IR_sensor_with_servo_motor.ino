#include <Servo.h>

int irPin = 2;         // IR sensor connected to digital pin 2
int servoPin = 9;      // Servo connected to pin 9
Servo myServo;

void setup() {
  pinMode(irPin, INPUT);
  myServo.attach(servoPin);
  myServo.write(0);    // Start at 0 degrees
  Serial.begin(9600);
}

void loop() {
  int irState = digitalRead(irPin);

  if (irState == LOW) {  // Object detected (LOW signal usually means detection)
    myServo.write(90);   // Rotate servo to 90 degrees
    Serial.println("Object Detected");
  } else {
    myServo.write(0);    // Rotate servo back to 0 degrees
    Serial.println("No Object");
  }

  delay(200);  // Small delay for stability
}
