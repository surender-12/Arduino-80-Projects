#include <Servo.h>

#define irSensor 7      // IR sensor signal pin
#define servoPin 9      // Servo signal pin
#define touchPin 6      // Touch sensor signal pin
#define buzzer 8        // Buzzer signal pin

Servo myServo;

void setup() {
  Serial.begin(9600);
  pinMode(irSensor, INPUT);
  pinMode(touchPin, INPUT);
  pinMode(buzzer, OUTPUT);
  myServo.attach(servoPin);
  myServo.write(0); // initial position

  Serial.println("Universal Code Ready");
}

void loop() {
  bool irDetected = false;
  bool touchDetected = false;

  // ---------- 1️⃣ IR SENSOR + SERVO ----------
  int irValue = digitalRead(irSensor);
  if (irValue == LOW) {   // IR sensor LOW = object detected (most modules)
    irDetected = true;
    Serial.println("IR MODE | Object Detected");
    myServo.write(180);
    delay(1000);
    myServo.write(0);
    delay(500);
  }

  // ---------- 2️⃣ TOUCH SENSOR + BUZZER ----------
  int touchValue = digitalRead(touchPin);
  if (touchValue == HIGH) {   // HIGH = touched
    touchDetected = true;
    digitalWrite(buzzer, HIGH);
    Serial.println("TOUCH MODE | Touch Detected");
  } else {
    digitalWrite(buzzer, LOW);
  }

  // ---------- NO SENSOR ACTIVE ----------
  if (!irDetected && !touchDetected) {
    myServo.write(0);
    digitalWrite(buzzer, LOW);
  }

  delay(100);
}
