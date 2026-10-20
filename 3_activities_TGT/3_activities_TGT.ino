#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// -------------------- Pins --------------------
#define DHTPIN 2
#define DHTTYPE DHT11
#define gasPin A0
#define touchPin 7
#define buzzer 8

// -------------------- Objects --------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);   // use 0x3F if 0x27 doesn’t work
DHT dht(DHTPIN, DHTTYPE);

// -------------------- Setup --------------------
void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  dht.begin();
  pinMode(buzzer, OUTPUT);
  pinMode(gasPin, INPUT);
  pinMode(touchPin, INPUT);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Multi Sensor");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(1500);
  lcd.clear();
}

// -------------------- Loop --------------------
void loop() {
  bool activeMode = false;

  // ---------- 1️⃣ DHT11 MODE ----------
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  if (!isnan(hum) && !isnan(temp)) {
    activeMode = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT MODE");
    delay(800);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.print(temp);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Hum:");
    lcd.print(hum);
    lcd.print("%");

    Serial.print("DHT MODE | Temp: ");
    Serial.print(temp);
    Serial.print(" C  Hum: ");
    Serial.println(hum);
    delay(2000);
    return;
  }

  // ---------- 2️⃣ GAS SENSOR MODE ----------
  int gasValue = analogRead(gasPin);
  if (gasValue > 5) {  // avoid random noise
    activeMode = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GAS MODE");
    delay(800);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas Value:");
    lcd.print(gasValue);

    if (gasValue > 400) {
      digitalWrite(buzzer, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("DANGER !!!");
    } else {
      digitalWrite(buzzer, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Good Air");
    }

    Serial.print("GAS MODE | Gas: ");
    Serial.println(gasValue);
    delay(1000);
    return;
  }

  // ---------- 3️⃣ TOUCH SENSOR MODE ----------
  int touchState = digitalRead(touchPin);
  if (touchState == HIGH || activeMode == false) {
    activeMode = true;
    if (touchState == HIGH) {
      digitalWrite(buzzer, HIGH);
      Serial.println("TOUCH MODE | Touch Detected");
    } else {
      digitalWrite(buzzer, LOW);
      Serial.println("TOUCH MODE | No Touch");
    }
    delay(300);
  }
}
