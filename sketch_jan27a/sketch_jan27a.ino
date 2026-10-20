#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD (I2C Address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int soilMoisturePin = A0;  // Soil sensor analog pin

void setup() {
    lcd.begin();
    lcd.backlight();  // Turn on LCD backlight
    Serial.begin(9600);
    lcd.setCursor(0, 0);
    lcd.print("Soil Moisture:");
}

void loop() {
    int moistureValue = analogRead(soilMoisturePin); // Read moisture sensor
    int percentage = map(moistureValue, 1023, 0, 0, 100); // Convert to percentage

    lcd.setCursor(0, 1);
    lcd.print("Moisture: ");
    lcd.print(percentage);
    lcd.print("%   ");  // Extra spaces to clear old values

    Serial.print("Soil Moisture: ");
    Serial.print(percentage);
    Serial.println("%");

    delay(1000);  // Update every second
}
