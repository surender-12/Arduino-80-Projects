#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD address may be 0x27 or 0x3F — adjust if needed
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16x2 LCD

void setup() {
  Serial.begin(9600);        // Start serial communication
  lcd.begin();               // Initialize LCD
  lcd.backlight();           // Turn on LCD backlight

  lcd.setCursor(0, 0);
  lcd.print("Happy Journey");

  lcd.setCursor(4, 1);       // Optional smiley on 2nd line
  lcd.print(":)");

  Serial.println("LCD Display: Happy Journey :)");
}

void loop() {
  // No repeated tasks needed here
}
