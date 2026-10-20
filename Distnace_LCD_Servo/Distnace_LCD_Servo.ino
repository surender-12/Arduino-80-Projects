#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo setup
Servo myServo;

// Ultrasonic pins
const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

void setup() {
  // Initialize LCD
  lcd.begin();
  lcd.backlight();

  // Ultrasonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Servo pin
  myServo.attach(6);

  lcd.setCursor(0, 0);
  lcd.print("Ultrasonic Test");
  delay(2000);
  lcd.clear();
}

void loop() {

  // Ultrasonic trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance
  distance = duration * 0.034 / 2;

  // Display distance on LCD
  lcd.setCursor(0, 0);
  lcd.print("Distance:      ");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");

  // Servo 0 to 180
  for (int pos = 0; pos <= 180; pos++) {
    myServo.write(pos);
    delay(10);
  }

  // Servo 180 to 0
  for (int pos = 180; pos >= 0; pos--) {
    myServo.write(pos);
    delay(10);
  }
}
