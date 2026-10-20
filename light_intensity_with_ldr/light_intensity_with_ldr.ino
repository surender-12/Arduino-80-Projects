#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Update 0x27 if your I2C address is different

const int ldrPin = A0;

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Light Intensity");
}

void loop() {
  int ldrValue = analogRead(ldrPin); // Range: 0 (bright) to 1023 (dark)
  int percentage = map(ldrValue, 0, 1023, 100, 0); // Reversed mapping

  lcd.setCursor(0, 1);
  lcd.print("Intensity: ");
  lcd.print(percentage);
  lcd.print("%   "); // Clears leftover characters

  delay(500);
}
