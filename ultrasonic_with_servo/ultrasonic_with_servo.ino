#include <Servo.h>

// Ultrasonic sensor pins
const int trigPin = 8; // Trigger pin
const int echoPin = 9; // Echo pin

// Create a Servo object
Servo myServo;

// Servo pin
const int servoPin = 10;

// Variables for ultrasonic sensor
long duration;
float distance;

void setup() {
  // Initialize the serial monitor
  Serial.begin(9600);

  // Set ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Attach the servo to the pin
  myServo.attach(servoPin);

  // Move servo to the initial position (0 degrees)
  myServo.write(0);
  delay(1000);
}

void loop() {
  // Sweep the servo from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle += 10) {
    myServo.write(angle); // Move servo to the current angle
    delay(300);           // Wait for the servo to reach the position
    distance = measureDistance(); // Measure distance
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("°, Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  // Sweep the servo back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle -= 10) {
    myServo.write(angle); // Move servo to the current angle
    delay(300);           // Wait for the servo to reach the position
    distance = measureDistance(); // Measure distance
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("°, Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  delay(1000); // Pause before repeating
}

// Function to measure distance using the ultrasonic sensor
float measureDistance() {
  // Send a 10-microsecond pulse to the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin and calculate distance
  duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.034) / 2; // Convert to centimeters

  return distance;
}
