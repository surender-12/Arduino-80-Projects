#include <Servo.h>

// Define servos
Servo baseServo;
Servo armServo;

// Define joystick pins
const int joystick1 = A0; // Joystick for base
const int joystick2 = A1; // Joystick for arm

// Define servo pins
const int baseServoPin = 9;
const int armServoPin = 10;

// Variables for joystick values
int baseValue;
int armValue;

void setup() {
  // Attach servos to pins
  baseServo.attach(baseServoPin);
  armServo.attach(armServoPin);

  // Set initial positions
  baseServo.write(90); // Center position
  armServo.write(90); // Center position

  // Initialize serial monitor for debugging
  Serial.begin(9600);
}

void loop() {
  // Read joystick values
  baseValue = analogRead(joystick1);
  armValue = analogRead(joystick2);

  // Map joystick values to servo positions (0-180 degrees)
  int baseAngle = map(baseValue, 0, 1023, 0, 180);
  int armAngle = map(armValue, 0, 1023, 0, 180);

  // Write values to servos
  baseServo.write(baseAngle);
  armServo.write(armAngle);

  // Debugging output
  Serial.print("Base Angle: ");
  Serial.print(baseAngle);
  Serial.print("\tArm Angle: ");
  Serial.println(armAngle);

  // Small delay for stability
  delay(15);
}


 
