// --- Pin configuration ---
int ir1Pin = 2;     // First IR sensor OUT pin
int ir2Pin = 3;     // Second IR sensor OUT pin

int led1 = 4;
int led2 = 5;
int led3 = 6;
int led4 = 7;

void setup() {
  pinMode(ir1Pin, INPUT);
  pinMode(ir2Pin, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int ir1Status = digitalRead(ir1Pin);  // 1st IR
  int ir2Status = digitalRead(ir2Pin);  // 2nd IR

  // Assuming IR gives LOW when object is detected
  if (ir1Status == LOW) {
    // Object at first IR -> turn LEDs ON
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    Serial.println("IR1 detected - LEDs ON");
  }

  if (ir2Status == LOW) {
    // Object at second IR -> turn LEDs OFF
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    Serial.println("IR2 detected - LEDs OFF");
  }

  delay(50); // small delay for stability
}
