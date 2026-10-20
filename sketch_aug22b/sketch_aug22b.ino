#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Ultrasonic sensor pins
const int trigPin = 6;
const int echoPin = 7;
long duration;
int distance;

// Servo setup
Servo myServo;  
const int servoPin = 9;   

// LED and Buzzer pins
const int ledPin = 13;
const int buzzerPin = 11;

// Timers
unsigned long previousLCD = 0;
unsigned long previousServo = 0;
unsigned long previousBlink = 0;

int servoPos = 0;
int servoStep = 90;  // Move between 0,90,180

void setup() {
  lcd.begin();
  lcd.backlight();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myServo.attach(servoPin);

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Ultrasonic");
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  // --- Ultrasonic & LCD (update every 200 ms) ---
  if (currentMillis - previousLCD >= 200) {
    previousLCD = currentMillis;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH, 20000); // timeout 20ms
    distance = duration * 0.034 / 2;

    lcd.setCursor(0, 0);
    lcd.print("Distance:      ");
    lcd.setCursor(0, 1);
    lcd.print(distance);
    lcd.print(" cm   ");
  }

  // --- Servo sweep (every 1s) ---
  if (currentMillis - previousServo >= 1000) {
    previousServo = currentMillis;

    myServo.write(servoPos);
    servoPos += servoStep;

    if (servoPos >= 180 || servoPos <= 0) {
      servoStep = -servoStep;  // Reverse direction
    }
  }

  // --- LED & Buzzer blink (every 500 ms) ---
  if (currentMillis - previousBlink >= 500) {
    previousBlink = currentMillis;

    digitalWrite(ledPin, !digitalRead(ledPin));
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));
  }
}
