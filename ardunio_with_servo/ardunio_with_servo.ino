#include <Servo.h>

// Create a Servo object
Servo myServo;

// Define the pin to which the servo is connected
const int servoPin = 9; // Servo signal pin connected to pin 9

void setup() {
  // Attach the servo object to the pin
  myServo.attach(servoPin);

  // Move the servo to the initial position (0 degrees)
  myServo.write(0);
  delay(1000); // Wait for the servo to reach the position
}

void loop() {
  // Move the servo to 0 degrees
  myServo.write(0);
  delay(1000); // Wait for 1 second

  // Move the servo to 90 degrees
  myServo.write(90);
  delay(1000); // Wait for 1 second

  // Move the servo to 180 degrees
  myServo.write(180);
  delay(1000); // Wait for 1 second

  // Move back to 90 degrees
  myServo.write(90);
  delay(1000); // Wait for 1 second

  // Move back to 0 degrees
  myServo.write(0);
  delay(1000); // Wait for 1 second
}
