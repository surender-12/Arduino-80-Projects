#include <Servo.h>

// ---------- Arduino B: RECEIVER ----------
// Receives object status via 433MHz RF, controls traffic light + servo

#include <RH_ASK.h>
#include <SPI.h>   // Required by RadioHead
#include <Servo.h>

RH_ASK driver;    // Uses default pins: RX = D11

// Traffic light pins
const int RED_PIN    = 2;
const int YELLOW_PIN = 3;
const int GREEN_PIN  = 4;

// Servo pin
const int SERVO_PIN  = 9;

Servo gateServo;

// State variables
bool detected     = false;  // current state (object present?)
bool lastDetected = false;  // previous state

void setLights(bool red, bool yellow, bool green) {
  digitalWrite(RED_PIN,    red    ? HIGH : LOW);
  digitalWrite(YELLOW_PIN, yellow ? HIGH : LOW);
  digitalWrite(GREEN_PIN,  green  ? HIGH : LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(RED_PIN,    OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN,  OUTPUT);

  gateServo.attach(SERVO_PIN);
  gateServo.write(0);            // Start at 0 degrees (open)

  if (!driver.init()) {
    Serial.println("RF init failed!");
  } else {
    Serial.println("RF init OK");
  }

  // Initially: No object → GREEN
  setLights(false, false, true);
}

void loop() {
  // Save previous state before reading new packet
  lastDetected = detected;

  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  // If a packet is received...
  if (driver.recv(buf, &buflen)) {
    if (buflen > 0) {
      char stateChar = (char)buf[0];
      detected = (stateChar == '1');   // '1' = object, '0' = no object
    }
  }

  // ----- Traffic light + servo logic -----

  if (detected) {
    // OBJECT PRESENT
    // -> RED ON, others OFF, servo 90 degrees
    setLights(true, false, false);
    gateServo.write(90);

    Serial.println("Object detected: RED, servo 90°");

  } else {
    // NO OBJECT
    if (lastDetected && !detected) {
      // Just changed from object present -> no object
      // RED → YELLOW (2s) → GREEN, servo back to 0 degrees

      Serial.println("Object just removed: YELLOW for 2s, then GREEN");

      // YELLOW
      setLights(false, true, false);
      // Keep servo at previous position (90°) during yellow, or you can
      // move it now if you prefer.
      delay(2000);

      // GREEN and servo to 0°
      setLights(false, false, true);
      gateServo.write(0);

    } else {
      // Still no object (and previously also no object)
      // Keep GREEN on and servo at 0°
      setLights(false, false, true);
      gateServo.write(0);
    }
  }

  delay(50);  // Slight delay to reduce serial spam and CPU usage
}
