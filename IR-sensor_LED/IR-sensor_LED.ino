// --- Pin configuration ---
int irPin = 2;     // IR sensor OUT pin
int led1 = 4;      // LED 1
int led2 = 5;      // LED 2
int led3 = 6;      // LED 3
int led4 = 7;   // Uncomment and use if you need a 4th LED

void setup() {
  pinMode(irPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  // pinMode(led4, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int irStatus = digitalRead(irPin);

  if (irStatus == LOW) {
    // Object detected (hand close to sensor)
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    Serial.println("Hand Detected!");
  } else {
    // No object
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
  }

  delay(100);
}
