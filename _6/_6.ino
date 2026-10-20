#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup (address might be 0x27 or 0x3F depending on your module)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensorIn = 2;
const int sensorOut = 3;

int peopleCount = 0;
bool sensorInTriggered = false;
bool sensorOutTriggered = false;

void setup() {
  pinMode(sensorIn, INPUT);
  pinMode(sensorOut, INPUT);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Crowd Counter");
  lcd.setCursor(0, 1);
  lcd.print("Count: 0");

  Serial.begin(9600);
}

void loop() {
  // Read sensors
  int inState = digitalRead(sensorIn);
  int outState = digitalRead(sensorOut);

  // Entry detection: IN → OUT
  if (inState == LOW) { // LOW means triggered
    sensorInTriggered = true;
    delay(300); // Debounce
  }
  if (sensorInTriggered && outState == LOW) {
    peopleCount++;
    sensorInTriggered = false;
    updateDisplay();
    delay(500); // Wait for person to pass
  }

  // Exit detection: OUT → IN
  if (outState == LOW) {
    sensorOutTriggered = true;
    delay(300); // Debounce
  }
  if (sensorOutTriggered && inState == LOW) {
    if (peopleCount > 0) peopleCount--;
    sensorOutTriggered = false;
    updateDisplay();
    delay(500); // Wait for person to pass
  }
}

void updateDisplay() {
  lcd.setCursor(0, 1);
  lcd.print("Count: ");
  lcd.print(peopleCount);
  lcd.print("     "); // Clear trailing chars
  Serial.println("People Count: " + String(peopleCount));
}
