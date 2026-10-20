#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define RAIN_PIN A0
#define LDR_DO_PIN 3

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  dht.begin();
  lcd.begin();
  lcd.backlight();
  pinMode(LDR_DO_PIN, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Weather Station");
  delay(2000);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  int rainRaw = analogRead(RAIN_PIN);
  int rainLevel = map(rainRaw, 1023, 0, 0, 100);  // Invert and map to 0-100%

  int lightStatus = digitalRead(LDR_DO_PIN);

  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(rainLevel);
  lcd.print("% ");

  lcd.print("L:");
  if (lightStatus == HIGH) {
    lcd.print("Bright");
  } else {
    lcd.print("Dark  ");
  }

  delay(2000);
}
