#include <Servo.h>

Servo myServo; // Create a servo object

void setup() {
  myServo.attach(9); // Attach servo to pin 9
}

void loop() {
  myServo.write(90); // Move to 90 degrees
  delay(2000);       // Wait for 2 seconds
  
  myServo.write(180); // Move to 180 degrees
  delay(2000);        // Wait for 2 seconds
}
