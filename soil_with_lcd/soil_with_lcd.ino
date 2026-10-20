#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SOIL_MOISTURE_PIN A0  // Soil sensor connected to A0

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set LCD address

void setup() {
    lcd.begin();
    lcd.backlight();
    Serial.begin(9600);
}

void loop() {
    int moistureValue = analogRead(SOIL_MOISTURE_PIN);  // Read soil sensor
    int percentage = map(moistureValue, 1023, 0, 0, 100);  // Convert to percentage

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Soil Moisture:");
    lcd.setCursor(0, 1);
    lcd.print(percentage);
    lcd.print("%");

    Serial.print("Soil Moisture: ");
    Serial.print(percentage);
    Serial.println("%");

    delay(1000);
}
