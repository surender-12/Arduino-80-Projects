#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// CHANGE THIS IF YOUR LCD HAS DIFFERENT ADDRESS (0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Bluetooth: Arduino Pin 2 = RX, Pin 3 = TX
SoftwareSerial bt(2, 3);

char buffer[100];
int bufPos = 0;

void setup() {
  Serial.begin(9600);
  bt.begin(9600);

  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wireless Display");
  delay(1500);
  lcd.clear();

  lcd.print("Waiting...");
}

void loop() {
  // Read incoming data from Bluetooth
  while (bt.available()) {
    char c = (char)bt.read();
    Serial.write(c);   // debug to serial

    if (c == '\r') continue;  // ignore CR

    if (c == '\n' || bufPos >= 98) {
      buffer[bufPos] = '\0';   // terminate string
      processMessage(String(buffer));
      bufPos = 0;
    }
    else {
      buffer[bufPos++] = c;
    }
  }
}

void processMessage(String msg) {
  msg.trim();
  if (msg.length() == 0) return;

  // Print same message to Serial
  Serial.println("Received: " + msg);

  // Display stays on LCD until a NEW message arrives
  lcd.clear();

  // If message is short, show on single line
  if (msg.length() <= 16) {
    lcd.setCursor(0, 0);
    lcd.print(msg);
    return;
  }

  // If message is long, split across two lines
  lcd.setCursor(0, 0);
  lcd.print(msg.substring(0, 16));

  if (msg.length() > 16) {
    lcd.setCursor(0, 1);
    lcd.print(msg.substring(16, msg.length()));
  }
}
