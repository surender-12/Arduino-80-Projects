#include <Servo.h>

Servo myServo;

int soundPin = 2;
int soundState;

void setup() {
  myServo.attach(9);
  pinMode(soundPin, INPUT);

  myServo.write(0);
}

void loop() {
  soundState = digitalRead(soundPin);

  if (soundState == HIGH) {
    myServo.write(90);
    delay(1000);

    myServo.write(0);
    delay(500);
  }
}