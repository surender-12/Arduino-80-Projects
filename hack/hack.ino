#include <Wire.h>                 // Comes with Arduino IDE                                 
#include <LiquidCrystal_I2C.h>
                                  // Initialize the object with interfacing pins 
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
 
void setup()
{
  Serial.begin(9600);             // Initialize serial communication at 9600 baudrate
 
  lcd.begin(16,2);                // Initialize 20X4 LCD and turn on backlight 
  lcd.backlight(); 
 
  lcd.setCursor(0,0);             // Set cursor to column 1, line 1 
  lcd.write(" 16x2 character ");
  lcd.setCursor(0,1);             // Set cursor to column 1, line 2 
  lcd.write("     I2C LCD    ");
 
  delay(2000);                    // Display for sometime
  lcd.noBacklight();              // Turn off backlight
  delay(2000);                    // Turn on backlight again after sometime
  lcd.backlight();            
 
  lcd.clear();                    
  lcd.setCursor(0,0);
  lcd.write("Use Serial Mon");
  lcd.setCursor(0,1);
  lcd.write("Type to display");  
}

 
void loop()                             // Setup - Runs continuously
{
  if(Serial.available())                // True if any character has arrived
  {
    lcd.clear();                        // Clear the display
    while (Serial.available() > 0) 
    {
      lcd.write(Serial.read());         // Display all the characters on LCD
    }
  }
}
