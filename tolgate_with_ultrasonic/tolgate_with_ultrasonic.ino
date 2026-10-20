#include <Servo.h>

const int trigPin = 9;  // Trig pin of ultrasonic sensor
const int echoPin = 10; // Echo pin of ultrasonic sensor
const int distanceThreshold = 15; // Distance threshold in cm

Servo tollGate;  // Create a servo object

void setup() {
  tollGate.attach(6);  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  tollGate.write(90);  // Default: Gate is OPEN (90°)
}

void loop() {
  // Measure distance using ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  int duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;  // Convert to cm

  if (distance > 0 && distance <= distanceThreshold) { // If vehicle detected
    tollGate.write(0);   // Close gate (0 degrees)
    delay(5000);         // Wait for 5 seconds
    tollGate.write(90);  // Reopen gate (90 degrees)
  }

  delay(500); // Small delay to prevent continuous triggering
}
