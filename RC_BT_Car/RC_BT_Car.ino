// Motor pins (PWM-capable as requested)
const uint8_t IN1 = 5;   // Motor A input 1 (PWM)
const uint8_t IN2 = 6;   // Motor A input 2 (PWM)
const uint8_t IN3 = 10;  // Motor B input 3 (PWM)
const uint8_t IN4 = 11;  // Motor B input 4 (PWM)

// NOTE: HC-05 wired to hardware Serial (pins 0 and 1):
// HC-05 RX -> Arduino TX (pin 1)  <--  use a voltage divider on HC-05 RX (Arduino TX) if needed
// HC-05 TX -> Arduino RX (pin 0)

char incoming = 0;

// Speed control
int speedLevel = 8;        // 0..9 default
uint8_t targetSpeed = 200;   // 0..255 (mapped from level)
uint8_t currentSpeed = 6;  // actual PWM value used
const uint16_t rampDelay = 5; // ms delay between ramp steps (smaller = faster ramp)

// Persistent last action so robot keeps moving after a command
// 'F' 'B' 'L' 'R' 'S'
char lastAction = 'S';

// Helper: map level 0..9 to PWM 0..255
uint8_t levelToPWM(int level) {
  level = constrain(level, 0, 9);
  return (uint8_t)map(level, 0, 9, 0, 255);
}

// Motor control helpers
void stopCoast() {
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
  if (dir > 0) {
    analogWrite(IN1, speed);
    analogWrite(IN2, 0);
  } else if (dir < 0) {
    analogWrite(IN1, 0);
    analogWrite(IN2, speed);
  } else {
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

// Movement functions (use currentSpeed)
void moveForward() {
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
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600); // hardware serial for HC-05 on pins 0/1 (and USB when HC-05 disconnected)

  // initialize speeds
  targetSpeed = levelToPWM(speedLevel);
  currentSpeed = 0; // start stopped
  stopCoast();

  Serial.println(F("Ready. Commands: F B L R S X 0-9 + -"));
}

void rampStep() {
  if (currentSpeed < targetSpeed) {
    currentSpeed++;
    delay(rampDelay);
  } else if (currentSpeed > targetSpeed) {
    currentSpeed--;
    delay(rampDelay);
  }
}

void applyLastAction() {
  switch (lastAction) {
    case 'F':
      moveForward();
      break;
    case 'B':
      moveBackward();
      break;
    case 'L':
      turnRight(); // 'L' was mapped before as pivot right in your earlier code; keep that mapping
      break;
    case 'R':
      turnLeft();  // 'R' mapped to pivot left earlier; keep same mapping
      break;
    case 'S':
    default:
      if (targetSpeed == 0) stopCoast();
      break;
  }
}

void processCommand(char cmd) {
  if (cmd >= '0' && cmd <= '9') {
    speedLevel = cmd - '0';
    targetSpeed = levelToPWM(speedLevel);
    Serial.print(F("Speed level set to "));
    Serial.print(speedLevel);
    Serial.print(F(" -> PWM "));
    Serial.println(targetSpeed);
    return;
  }

  switch (cmd) {
    case '+':
      speedLevel = min(speedLevel + 1, 9);
      targetSpeed = levelToPWM(speedLevel);
      Serial.print(F("Speed + -> "));
      Serial.println(speedLevel);
      break;

    case '-':
      speedLevel = max(speedLevel - 1, 0);
      targetSpeed = levelToPWM(speedLevel);
      Serial.print(F("Speed - -> "));
      Serial.println(speedLevel);
      break;

    case 'F': // forward
      Serial.println(F("Command: FORWARD"));
      lastAction = 'F';
      break;

    case 'B': // backward
      Serial.println(F("Command: BACKWARD"));
      lastAction = 'B';
      break;

    case 'L': // turn right (pivot)
      Serial.println(F("Command: TURN RIGHT"));
      lastAction = 'L';
      break;

    case 'R': // turn left (pivot)
      Serial.println(F("Command: TURN LEFT"));
      lastAction = 'R';
      break;

    case 'S': // stop (coast)
      Serial.println(F("Command: STOP (coast)"));
      lastAction = 'S';
      targetSpeed = 0;
      break;

    case 'X': // hard brake
      Serial.println(F("Command: HARD BRAKE"));
      hardBrake();
      // short brake pulse
      delay(120);
      stopCoast();
      currentSpeed = 0;
      targetSpeed = 0;
      lastAction = 'S';
      break;

    default:
      Serial.print(F("Unknown cmd: "));
      Serial.println(cmd);
      break;
  }
}

void loop() {
  // Read all available bytes and process them; hardware Serial used for HC-05
  while (Serial.available()) {
    incoming = (char)Serial.read();
    // ignore CR/LF
    if (incoming == '\r' || incoming == '\n') continue;
    processCommand(incoming);
  }

  // Ramp speed smoothly toward target
  if (currentSpeed != targetSpeed) {
    rampStep();
  }

  // Apply the current movement according to lastAction and currentSpeed
  applyLastAction();

  // brief loop delay for stability
  delay(3);
}
