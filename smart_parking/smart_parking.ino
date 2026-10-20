#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD address (0x27 or 0x3F, check your module)
// 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// IR sensor pins
int ir1 = 2;
int ir2 = 3;
int ir3 = 4;
int ir4 = 5;

void setup() {
  // Initialize LCD
  lcd.begin();
  lcd.backlight();

  // IR sensor inputs
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);

  // Welcome message
  lcd.setCursor(0, 0);
  lcd.print(" Smart Parking ");
  lcd.setCursor(0, 1);
  lcd.print("   System ON   ");
  delay(2000);  // Show for 2 seconds

  lcd.clear();
}

void loop() {
  // Read IR sensors
  int s1 = digitalRead(ir1);
  int s2 = digitalRead(ir2);
  int s3 = digitalRead(ir3);
  int s4 = digitalRead(ir4);

  // First row → Slot 1 and Slot 2
  lcd.setCursor(0, 0);
  lcd.print("S1:");
  lcd.print(s1 == LOW ? "P" : "E");   // IR LOW = Object detected = Parked
  lcd.print("  ");

  lcd.print("S2:");
  lcd.print(s2 == LOW ? "P" : "E");

  // Second row → Slot 3 and Slot 4
  lcd.setCursor(0, 1);
  lcd.print("S3:");
  lcd.print(s3 == LOW ? "P" : "E");   
  lcd.print("  ");

  lcd.print("S4:");
  lcd.print(s4 == LOW ? "P" : "E");

  delay(500);  // Refresh every half second
}
