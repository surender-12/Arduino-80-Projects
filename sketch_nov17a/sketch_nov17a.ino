/*
  Welcome + Bluetooth -> I2C LCD demo
  - Shows "Welcome to" / "Wireless display" on power-up
  - Then shows "Waiting for Msg" until connected or first data received
  - Displays incoming Bluetooth data (HC-05) on the LCD
  - Auto-tries common I2C addresses 0x27 and 0x3F
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// SoftwareSerial pins (HC-05)
const int bluetoothRX = 2; // HC-05 TX -> Arduino 2
const int bluetoothTX = 3; // HC-05 RX <- Arduino 3 (use voltage divider)
SoftwareSerial bluetooth(bluetoothRX, bluetoothTX);

// If your HC-05 exposes a STATE pin, wire it here and set pin number.
// If not available, set to -1 to use fallback (first-data detection).
const int HC05_STATE_PIN = -1; // set to e.g. 4 if you wired STATE -> D4

// candidate I2C addresses
const uint8_t i2cCandidates[] = {0x27, 0x3F};
LiquidCrystal_I2C *lcd = nullptr;
uint8_t lcdAddrFound = 0;

String message = "";
unsigned long lastReceive = 0;
const unsigned long MESSAGE_TIMEOUT = 200; // ms
bool shownWireless = false;

void tryInitLCD(uint8_t addr) {
  if (lcd) { delete lcd; lcd = nullptr; }
  lcd = new LiquidCrystal_I2C(addr, 16, 2);
  lcd->begin();
  lcd->backlight();
  delay(50);
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  Wire.begin();
  if (HC05_STATE_PIN >= 0) pinMode(HC05_STATE_PIN, INPUT);

  // detect I2C addr
  for (uint8_t i = 0; i < sizeof(i2cCandidates); ++i) {
    uint8_t addr = i2cCandidates[i];
    Wire.beginTransmission(addr);
    uint8_t err = Wire.endTransmission();
    if (err == 0) { lcdAddrFound = addr; break; }
  }
  if (lcdAddrFound == 0) lcdAddrFound = 0x27; // fallback

  tryInitLCD(lcdAddrFound);
  Serial.print("LCD at 0x"); Serial.println(lcdAddrFound, HEX);

  // Power-up welcome message
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Welcome to");
  lcd->setCursor(0, 1);
  lcd->print("Wireless display");
  delay(2500); // show welcome for 2.5 seconds

  // then show waiting screen
  showWaiting();
  Serial.println("Startup done. Waiting for BT connection/data.");
}

void showWirelessDisplay() {
  if (!lcd) return;
  lcd->clear();
  lcd->setCursor(0,0);
  lcd->print("Wireless display");
  lcd->setCursor(0,1);
  lcd->print("Connected");
  shownWireless = true;
}

void showWaiting() {
  if (!lcd) return;
  lcd->clear();
  lcd->setCursor(0,0);
  lcd->print("Waiting for Msg");
  lcd->setCursor(0,1);
  lcd->print("BT: Not connected");
  shownWireless = false;
}

void loop() {
  // If STATE pin used, detect connection changes
  if (HC05_STATE_PIN >= 0) {
    static bool lastState = false;
    bool s = (digitalRead(HC05_STATE_PIN) == HIGH);
    if (s != lastState) {
      lastState = s;
      if (s) showWirelessDisplay(); else showWaiting();
    }
  }

  // Read incoming bluetooth bytes
  while (bluetooth.available()) {
    char c = (char)bluetooth.read();
    if (c == '\r') continue;
    message += c;
    lastReceive = millis();
    Serial.print("rx char: "); Serial.println(c);

    // fallback: if no STATE pin, treat first data as "connected"
    if (HC05_STATE_PIN < 0 && !shownWireless) showWirelessDisplay();
  }

  // If message complete (no new char for MESSAGE_TIMEOUT), display it
  if (message.length() > 0 && millis() - lastReceive > MESSAGE_TIMEOUT) {
    Serial.print("Full message: "); Serial.println(message);

    // Prepare two lines
    String firstLine, secondLine;
    int nl = message.indexOf('\n');
    if (nl >= 0) {
      firstLine = message.substring(0, nl);
      secondLine = message.substring(nl + 1);
    } else {
      if (message.length() <= 16) {
        firstLine = message;
        secondLine = "";
      } else {
        firstLine = message.substring(0, 16);
        secondLine = message.substring(16);
        if (secondLine.length() > 16) secondLine = secondLine.substring(0, 16);
      }
    }

    lcd->clear();
    lcd->setCursor(0,0); lcd->print(firstLine);
    lcd->setCursor(0,1); lcd->print(secondLine);

    // after showing message, return to Wireless header if using fallback
    if (HC05_STATE_PIN < 0) {
      delay(1500);
      lcd->clear();
      lcd->setCursor(0,0); lcd->print("Wireless display");
      lcd->setCursor(0,1); lcd->print("Ready to receive");
      shownWireless = true;
    }

    message = "";
  }
}
