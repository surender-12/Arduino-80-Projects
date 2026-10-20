#include <NewPing.h>

#define ULTRASONIC_SENSOR_TRIG 11
#define ULTRASONIC_SENSOR_ECHO 12

#define MAX_FORWARD_MOTOR_SPEED 150      // PWM 0-255 (tune as needed)
#define MAX_MOTOR_TURN_SPEED_ADJUSTMENT 80

#define MIN_DISTANCE 2
#define MAX_DISTANCE 30                  // obstacle threshold in cm

#define IR_SENSOR_RIGHT 2
#define IR_SENSOR_LEFT 3

// Right motor
int enableRightMotor = 6;
int rightMotorPin1 = 9;
int rightMotorPin2 = 10;

// Left motor
int enableLeftMotor = 5;
int leftMotorPin1 = 8;
int leftMotorPin2 = 7;

NewPing mySensor(ULTRASONIC_SENSOR_TRIG, ULTRASONIC_SENSOR_ECHO, 400);

void setup() {
  Serial.begin(9600);

  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);

  // Ensure motors stopped initially
  rotateMotor(0, 0);
}

void loop() {
  // Read distance (cm). NewPing returns 0 when no ping — treat as large distance.
  int rawDistance = mySensor.ping_cm();
  int distance = (rawDistance == 0) ? 500 : rawDistance; // treat 0 as "no object"
  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");

  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
  int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);

  // Object-avoidance priority:
  // If an object is closer than MAX_DISTANCE -> stop, back up, turn, then continue.
  if (distance <= MAX_DISTANCE && distance >= MIN_DISTANCE) {
    Serial.println("Obstacle detected! Executing avoidance maneuver...");
    // Stop first
    rotateMotor(0, 0);
    delay(150);

    // Reverse for a short duration
    rotateMotor(-120, -120); // reverse both motors (negative -> reverse)
    delay(450);

    // Turn in place (choose direction). Here we choose direction based on IR sensors if available,
    // otherwise alternate turn direction to avoid getting stuck.
    if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH) {
      // If right IR saw something (LOW), turn left to avoid
      rotateMotor(-120, 120); // rotate left (right motor reverse, left forward)
      delay(450);
    } else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW) {
      // If left IR saw something, turn right to avoid
      rotateMotor(120, -120); // rotate right
      delay(450);
    } else {
      // No IR preference: choose a default turn direction or alternate
      static bool turnLeftNext = true;
      if (turnLeftNext) {
        rotateMotor(-120, 120); // left turn
      } else {
        rotateMotor(120, -120); // right turn
      }
      turnLeftNext = !turnLeftNext;
      delay(450);
    }

    // Short pause then continue
    rotateMotor(0, 0);
    delay(150);

    // Resume forward motion (will be handled below in next loop)
  }
  else {
    // No close obstacle -> use IR-based gesture turning or go straight
    // NOTE: IR sensors: LOW = detected, HIGH = not detected
    if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH) {
      // Turn right (increase left motor speed, reduce right motor speed)
      Serial.println("IR: turn right");
      rotateMotor(MAX_FORWARD_MOTOR_SPEED - MAX_MOTOR_TURN_SPEED_ADJUSTMENT,
                  MAX_FORWARD_MOTOR_SPEED + MAX_MOTOR_TURN_SPEED_ADJUSTMENT);
    }
    else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW) {
      // Turn left
      Serial.println("IR: turn left");
      rotateMotor(MAX_FORWARD_MOTOR_SPEED + MAX_MOTOR_TURN_SPEED_ADJUSTMENT,
                  MAX_FORWARD_MOTOR_SPEED - MAX_MOTOR_TURN_SPEED_ADJUSTMENT);
    }
    else {
      // No IR gesture: go straight at forward speed
      rotateMotor(MAX_FORWARD_MOTOR_SPEED, MAX_FORWARD_MOTOR_SPEED);
    }
  }

  delay(200); // small loop delay (tune as needed)
}

// rotateMotor: positive -> forward, negative -> reverse, 0 -> stop
void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
  // Right motor direction
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

  // Left motor direction
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

  // Ensure PWM values in 0-255 range
  int rightPWM = constrain(abs(rightMotorSpeed), 0, 255);
  int leftPWM = constrain(abs(leftMotorSpeed), 0, 255);

  analogWrite(enableRightMotor, rightPWM);
  analogWrite(enableLeftMotor, leftPWM);
}