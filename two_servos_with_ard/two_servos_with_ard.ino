#include <Servo.h>

// Create Servo objects
Servo servo1;  // First servo
Servo servo2;  // Second servo

// Define servo pins
const int servo1Pin = 3; // Servo 1 connected to pin 3
const int servo2Pin = 5; // Servo 2 connected to pin 5

void setup() {
  // Attach servos to the defined pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Move both servos to initial positions
  servo1.write(0);  // Servo 1 starts at 0 degrees
  servo2.write(0);  // Servo 2 starts at 0 degrees
  delay(1000);      // Wait for 1 second
}

void loop() {
  // Move servo 1 from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle += 10) {
    servo1.write(angle);
    delay(100); // Wait for 100 ms
  }

  // Move servo 2 from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle += 10) {
    servo2.write(angle);
    delay(100); // Wait for 100 ms
  }

  // Move servo 1 back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle -= 10) {
    servo1.write(angle);
    delay(100); // Wait for 100 ms
  }

  // Move servo 2 back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle -= 10) {
    servo2.write(angle);
    delay(100); // Wait for 100 ms
  }

  delay(2000); // Wait for 2 seconds before repeating the loop
}
