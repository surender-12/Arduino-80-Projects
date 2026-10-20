// Define pin connections
const int trigPin = 9; // Trigger pin
const int echoPin = 10; // Echo pin

// Define variables for duration and distance
long duration;
float distance;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set trigPin as output and echoPin as input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Send a 10-microsecond pulse to trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin and calculate the distance
  duration = pulseIn(echoPin, HIGH); // Measure the duration of the echo signal
  distance = (duration * 0.034) / 2; // Convert to distance in centimeters

  // Display the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Wait for a short time before repeating
  delay(500);
}
