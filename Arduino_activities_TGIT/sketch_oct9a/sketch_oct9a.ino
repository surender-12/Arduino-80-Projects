#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "DHT.h"

// -------------------- Pin Definitions --------------------
#define DHTPIN 2
#define DHTTYPE DHT11
#define gasPin A0
#define buzzer 11        // ✅ Changed from 8 → 11
#define irSensor 7
#define servoPin 9
#define touchPin 6

// -------------------- Objects --------------------.

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
Servo myServo;

// -------------------- Setup --------------------
void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  dht.begin();
  myServo.attach(servoPin);
  
  pinMode(buzzer, OUTPUT);
  pinMode(gasPin, INPUT);
  pinMode(irSensor, INPUT);
  pinMode(touchPin, INPUT);

  myServo.write(0);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Multi Sensor Sys");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(1500);
  lcd.clear();
}

// -------------------- Loop --------------------
void loop() {

  // ---------- 1️⃣ DHT11 MODE ----------
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  if (!isnan(hum) && !isnan(temp)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT MODE");
    delay(800);

    while (true) {  // Stay in DHT mode
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp:");
      lcd.print(t);
      lcd.print("C");

      lcd.setCursor(0, 1);
      lcd.print("Hum:");
      lcd.print(h);
      lcd.print("%");

      Serial.print("DHT MODE | Temp: ");
      Serial.print(t);
      Serial.print(" C  Hum: ");
      Serial.println(h);

      delay(2000);
    }
  }

  // ---------- 2️⃣ GAS SENSOR MODE ----------
  int gasValue = analogRead(gasPin);
  if (gasValue > 10) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GAS MODE");
    delay(800);

    while (true) {  // Stay in GAS mode
      int g = analogRead(gasPin);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gas Value:");
      lcd.print(g);

      if (g > 400) {
        digitalWrite(buzzer, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("DANGER !!!");
      } else {
        digitalWrite(buzzer, LOW);
        lcd.setCursor(0, 1);
        lcd.print("Good Air");
      }

      Serial.print("GAS MODE | Gas: ");
      Serial.println(g);
      delay(1000);
    }
  }

  // ---------- 3️⃣ IR SENSOR + SERVO MODE ----------
  int irValue = digitalRead(irSensor);
  if (irValue == LOW) { // Object detected
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("IR MODE");
    delay(800);

    while (true) {  // Stay in IR mode
      int ir = digitalRead(irSensor);
      if (ir == LOW) {
        Serial.println("IR MODE | Object Detected");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("IR MODE");
        lcd.setCursor(0, 1);
        lcd.print("Object Detected");
        myServo.write(180);
        delay(1000);
        myServo.write(0);
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("IR MODE");
        lcd.setCursor(0, 1);
        lcd.print("No Object");
        myServo.write(0);
      }
      delay(500);
    }
  }

  // ---------- 4️⃣ TOUCH SENSOR + BUZZER MODE ----------
  int touchState = digitalRead(touchPin);
  if (touchState == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TOUCH MODE");
    delay(800);

    while (true) {  // Stay in TOUCH mode
      int tState = digitalRead(touchPin);
      if (tState == HIGH) {
        digitalWrite(buzzer, HIGH);
        Serial.println("TOUCH MODE | Touch Detected");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("TOUCH MODE");
        lcd.setCursor(0, 1);
        lcd.print("Touch Detected");
      } else {
        digitalWrite(buzzer, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("TOUCH MODE");
        lcd.setCursor(0, 1);
        lcd.print("No Touch");
      }
      delay(400);
    }
  }

  delay(200);
}
