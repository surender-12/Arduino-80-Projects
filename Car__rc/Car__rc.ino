#include <SoftwareSerial.h>

// Pins (PWM-capable as requested)
const uint8_t IN1 = 5;   // Motor A input 1 (PWM)
const uint8_t IN2 = 6;   // Motor A input 2 (PWM)
const uint8_t IN3 = 10;  // Motor B input 3 (PWM)
const uint8_t IN4 = 11;  // Motor B input 4 (PWM)

// Bluetooth (HC-05) via SoftwareSerial
const uint8_t BT_RX = 2; // Arduino receives from HC-05 TX
const uint8_t BT_TX = 3; // Arduino transmits to HC-05 RX (use divider)
SoftwareSerial btSerial(BT_RX, BT_TX);

char dataChar = 0;

// Speed control
int speedLevel = 6;        // 0..9 default
uint8_t targetSpeed = 0;   // 0..255 (mapped from level)
uint8_t currentSpeed = 0;  // actual PWM value used
const uint16_t rampDelay = 12; // ms delay between ramp steps (smaller = faster ramp)

// Helper: map level 0..9 to PWM 0..255
uint8_t levelToPWM(int level) {
  level = constrain(level, 0, 9);
  return (uint8_t)map(level, 0, 9, 0, 255);
}

// Motor control helpers
void stopCoast() {
  // release motors (coast)
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

void hardBrake() {
  // Active braking: set both inputs HIGH for each motor
  analogWrite(IN1, 255);
  analogWrite(IN2, 255);
  analogWrite(IN3, 255);
  analogWrite(IN4, 255);
}

// Set Motor A: direction (1 = forward, -1 = reverse, 0 = stop), speed 0..255
void setMotorA(int dir, uint8_t speed) {
  if (dir > 0) {           // forward
    analogWrite(IN1, speed);
    analogWrite(IN2, 0);
  } else if (dir < 0) {    // reverse
    analogWrite(IN1, 0);
    analogWrite(IN2, speed);
  } else {                 // stop/coast
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
  }
}

// Set Motor B: direction (1 = forward, -1 = reverse, 0 = stop), speed 0..255
void setMotorB(int dir, uint8_t speed) {
  if (dir > 0) {
    analogWrite(IN3, speed);
    analogWrite(IN4, 0);
  } else if (dir < 0) {
    analogWrite(IN3, 0);
    analogWrite(IN4, speed);
  } else {
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
  }
}

// Ramp currentSpeed to targetSpeed smoothly
void rampToTarget() {
  if (currentSpeed == targetSpeed) return;
  if (currentSpeed < targetSpeed) {
    currentSpeed++;
  } else {
    currentSpeed--;
  }
  delay(rampDelay);
}

// Movement high-level functions (use currentSpeed value)
void moveForward() {
  // both motors forward
  setMotorA(1, currentSpeed);
  setMotorB(1, currentSpeed);
}

void moveBackward() {
  setMotorA(-1, currentSpeed);
  setMotorB(-1, currentSpeed);
}

void turnRight() {
  // left forward, right reverse -> pivot right
  setMotorA(1, currentSpeed);
  setMotorB(-1, currentSpeed);
}

void turnLeft() {
  // left reverse, right forward -> pivot left
  setMotorA(-1, currentSpeed);
  setMotorB(1, currentSpeed);
}

void setup() {
  // pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);      // USB debug
  btSerial.begin(9600);    // HC-05 default baud

  // initialize speeds
  targetSpeed = levelToPWM(speedLevel);
  currentSpeed = 0; // start from stopped
  stopCoast();

  Serial.println("Ready. Commands: F B L R S X 0-9 + -");
}

void loop() {
  // Read from Bluetooth (preferred) or USB serial (for testing)
  if (btSerial.available()) {
    dataChar = btSerial.read();
  } else if (Serial.available()) {
    dataChar = Serial.read();
  } else {
    dataChar = 0;
  }

  if (dataChar != 0) {
    // handle commands
    if (dataChar >= '0' && dataChar <= '9') {
      speedLevel = dataChar - '0';
      targetSpeed = levelToPWM(speedLevel);
      Serial.print("Speed level set to ");
      Serial.print(speedLevel);
      Serial.print(" -> PWM ");
      Serial.println(targetSpeed);
    } else {
      switch (dataChar) {
        case '+':
          speedLevel = min(speedLevel + 1, 9);
          targetSpeed = levelToPWM(speedLevel);
          Serial.print("Speed + -> ");
          Serial.println(speedLevel);
          break;
        case '-':
          speedLevel = max(speedLevel - 1, 0);
          targetSpeed = levelToPWM(speedLevel);
          Serial.print("Speed - -> ");
          Serial.println(speedLevel);
          break;
        case 'F': // forward
          Serial.println("Command: FORWARD");
          // ramp up while issuing move command in main loop
          break;
        case 'B': // backward
          Serial.println("Command: BACKWARD");
          break;
        case 'L': // turn right (pivot)
          Serial.println("Command: TURN RIGHT");
          break;
        case 'R': // turn left (pivot)
          Serial.println("Command: TURN LEFT");
          break;
        case 'S': // stop (coast)
          Serial.println("Command: STOP (coast)");
          targetSpeed = 0;
          break;
        case 'X': // hard brake
          Serial.println("Command: HARD BRAKE");
          hardBrake();
          currentSpeed = 0;
          targetSpeed = 0;
          delay(120); // short brake pulse
          stopCoast();
          break;
        default:
          Serial.print("Unknown cmd: ");
          Serial.println(dataChar);
          break;
      }
    }
    // clear
    dataChar = 0;
  }

  // Smoothly ramp currentSpeed to targetSpeed
  if (currentSpeed != targetSpeed) {
    rampToTarget();
    // After adjusting currentSpeed, we need to maintain direction previously commanded.
    // We will not remember the last movement command state separately; instead assume
    // the user regularly sends direction commands (F/B/L/R) to keep movement.
    // To make movement persistent, you can store lastDirection variable. Let's implement that below.
  }

  // -- persistent direction implementation (so robot continues once commanded) --
  // We'll store last action and keep applying with currentSpeed
  static char lastAction = 'S'; // default stopped
  // Check if the user sent a direction char on Serial/BT (btSerial/Serial read above only set dataChar once).
  // To capture direction persistently, we re-check incoming bytes quickly:
  if (btSerial.available()) lastAction = btSerial.read();
  else if (Serial.available()) lastAction = Serial.read();

  // If lastAction is numeric or +/- or unknown, don't change movement; only change on direction commands
  switch (lastAction) {
    case 'F':
      moveForward();
      break;
    case 'B':
      moveBackward();
      break;
    case 'L':
      turnRight();
      break;
    case 'R':
      turnLeft();
      break;
    case 'S':
      stopCoast();
      break;
    default:
      // If it's a numeric or +/- or something else, keep current motor outputs (no change).
      // But if targetSpeed == 0, ensure motors are released:
      if (targetSpeed == 0) stopCoast();
      break;
  }

  // small loop delay to keep ramp timing stable
  delay(3);
}
