#include <Servo.h>

#define SOUND_PIN 2    // Sound sensor OUT connected to D2
#define LED_PIN 13     // LED connected to D13
#define SERVO_PIN 9    // Servo connected to D9

Servo myServo;
bool servoAttached = false;

void setup() {
  pinMode(SOUND_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  myServo.attach(SERVO_PIN);
  myServo.write(0);
  servoAttached = true;
}

void loop() {
  int soundState = digitalRead(SOUND_PIN);

  if (soundState == HIGH) {
    digitalWrite(LED_PIN, HIGH);
    if (servoAttached) {
      myServo.write(180);
    }
  } else {
    digitalWrite(LED_PIN, LOW);
    if (servoAttached) {
      myServo.write(0);
    }
  }

  delay(10);
}
