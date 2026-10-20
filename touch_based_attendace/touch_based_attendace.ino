#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD I2C address (0x27 or 0x3F)

#define TOUCH_PIN 2   // Touch Sensor connected to Digital Pin 2
#define BUZZER_PIN 3  // Buzzer connected to Digital Pin 3

int rollNumber = 1; // Start from Roll No. 1

void setup() {
    pinMode(TOUCH_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    
    lcd.begin();
    lcd.backlight();
    
    lcd.setCursor(0, 0);
    lcd.print("Attendance");  // Initial message
}

void loop() {
    if (digitalRead(TOUCH_PIN) == HIGH) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Roll No: ");
        lcd.print(rollNumber); // Display roll number
        
        tone(BUZZER_PIN, 1000, 500); // Beep for 500ms

        delay(2000); // Display for 2 seconds
        
        rollNumber++; // Move to the next roll number
        
        if (rollNumber > 5) { // Reset to 1 after 5
            rollNumber = 1;
        }
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Attendance"); // Restore initial message
    }
}
