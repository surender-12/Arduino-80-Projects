#include <NewPing.h>

/* -----------------------
   Pin definitions (same as you specified)
   ----------------------- */
#define ULTRASONIC_SENSOR_TRIG 11
#define ULTRASONIC_SENSOR_ECHO 12

// Motor pins (L298N style)
const int enableRightMotor = 5;   // ENA (PWM) - Right motor speed
const int rightMotorPin1  = 7;    // IN1 - Right motor direction
const int rightMotorPin2  = 8;   // IN2 - Right motor direction

const int enableLeftMotor  = 6;   // ENB (PWM) - Left motor speed
const int leftMotorPin1    = 10;   // IN3 - Left motor direction
const int leftMotorPin2    = 9;   // IN4 - Left motor direction

/* -----------------------
   Movement / sensor config
   ----------------------- */
const int MAX_FORWARD_MOTOR_SPEED = 150; // 0-255 PWM for forward
const int MAX_DISTANCE = 30;             // cm -> obstacle threshold
const int MIN_DISTANCE = 2;              // ignore extremely close invalid reads

NewPing sonar(ULTRASONIC_SENSOR_TRIG, ULTRASONIC_SENSOR_ECHO, 400);

/* -----------------------
   State machine for avoidance (non-blocking)
   ----------------------- */
enum State { FORWARD, REVERSE, TURN, PAUSE };
State currentState = FORWARD;

unsigned long stateStartTime = 0;

// durations (ms) - tune these for your robot
const unsigned long REVERSE_DURATION = 450;
const unsigned long TURN_DURATION    = 600;
const unsigned long PAUSE_DURATION   = 200;

// turn direction toggler (alternates left/right each avoidance)
bool turnLeftNext = true;

// sensor read interval (ms)
const unsigned long SENSOR_INTERVAL = 100;
unsigned long lastSensorRead = 0;
int lastDistanceCm = 500; // large init value

void setup() {
  Serial.begin(9600);

  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  // Start stopped
  rotateMotor(0, 0);
  currentState = FORWARD;
  stateStartTime = millis();
}

void loop() {
  unsigned long now = millis();

  // Read the ultrasonic sensor at SENSOR_INTERVAL to avoid excessive pings
  if (now - lastSensorRead >= SENSOR_INTERVAL) {
    int raw = sonar.ping_cm();
    lastDistanceCm = (raw == 0) ? 500 : raw; // treat 0 (no echo) as "far"
    lastSensorRead = now;
    Serial.print("Distance: ");
    Serial.print(lastDistanceCm);
    Serial.println(" cm");
  }

  switch (currentState) {
    case FORWARD:
      // If an obstacle is detected within threshold -> start avoidance
      if (lastDistanceCm <= MAX_DISTANCE && lastDistanceCm >= MIN_DISTANCE) {
        Serial.println("Obstacle detected -> start avoidance");
        // Stop briefly, then reverse
        rotateMotor(0, 0);
        currentState = REVERSE;
        stateStartTime = now;
      } else {
        // Path clear - go forward
        rotateMotor(MAX_FORWARD_MOTOR_SPEED, MAX_FORWARD_MOTOR_SPEED);
      }
      break;

    case REVERSE:
      // Reverse both motors
      rotateMotor(-120, -120);
      if (now - stateStartTime >= REVERSE_DURATION) {
        // After reversing, go to TURN state
        currentState = TURN;
        stateStartTime = now;
      }
      break;

    case TURN:
      // Turn in place; alternate directions each avoidance
      if (turnLeftNext) {
        // turn left: right motor reverse, left motor forward
        rotateMotor(-120, 120);
      } else {
        // turn right: right motor forward, left motor reverse
        rotateMotor(120, -120);
      }
      if (now - stateStartTime >= TURN_DURATION) {
        // finished turn -> small pause then resume forward
        turnLeftNext = !turnLeftNext; // toggle for next time
        currentState = PAUSE;
        stateStartTime = now;
      }
      break;

    case PAUSE:
      // brief stop so the new heading stabilizes
      rotateMotor(0, 0);
      if (now - stateStartTime >= PAUSE_DURATION) {
        // resume forward and return to FORWARD state
        currentState = FORWARD;
        stateStartTime = now;
      }
      break;
  }
}

/* rotateMotor:
   positive speed -> forward
   negative speed -> reverse
   speed range used for PWM is 0-255 (constrain applied)
*/
void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
  // Right motor direction pins
  if (rightMotorSpeed < 0) {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  } else if (rightMotorSpeed > 0) {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  } else {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  // Left motor direction pins
  if (leftMotorSpeed < 0) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  } else if (leftMotorSpeed > 0) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  } else {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }

  // Apply PWM speeds (abs, constrained)
  int rPWM = constrain(abs(rightMotorSpeed), 0, 255);
  int lPWM = constrain(abs(leftMotorSpeed), 0, 255);
  analogWrite(enableRightMotor, rPWM);
  analogWrite(enableLeftMotor, lPWM);
}
