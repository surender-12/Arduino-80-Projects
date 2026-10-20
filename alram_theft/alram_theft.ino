// Define pins for ultrasonic sensor and buzzer
const int trigPin = 9; // Trigger pin of ultrasonic sensor
const int echoPin = 10; // Echo pin of ultrasonic sensor
const int buzzerPin = 7; // Buzzer pin

// Define the detection range (in centimeters)
const int detectionRange = 50; // Adjust this value as per requirement

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Configure pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Ensure buzzer is off initially
  digitalWrite(buzzerPin, LOW);

  Serial.println("Thief Alarm System Initialized!");
}

void loop() {
  // Send a pulse from the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the echo pulse
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance (in cm)
  int distance = duration * 0.034 / 2;

  // Print distance for debugging
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check if the distance is within the detection range
  if (distance > 0 && distance <= detectionRange) {
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    Serial.println("Intruder detected! Alarm triggered.");
  } else {
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
  }

  delay(200); // Delay for stability
}
