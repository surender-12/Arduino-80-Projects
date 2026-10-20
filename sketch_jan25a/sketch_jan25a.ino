#include <Servo.h>

Servo myservo;  // Create a Servo object

void setup() {
  myservo.attach(9);  // Pin 9 to control the servo motor
  myservo.write(0);   // Set servo to 0 degrees
  delay(2000);        // Wait for 2 seconds to ensure servo is at 0 degrees
}

void loop() {
  myservo.write(90);  // Rotate servo to 90 degrees
  delay(10000);       // Wait for 10 seconds
  myservo.write(0);   // Return servo to 0 degrees
  delay(2000);        // Wait for 2 seconds before repeating
}
