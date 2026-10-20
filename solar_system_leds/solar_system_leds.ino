// Define LED pins
const int mercuryLED = 2;
const int venusLED = 3;
const int earthLED = 4;
const int marsLED = 5;
const int jupiterLED = 6;
const int saturnLED = 7;
const int uranusLED = 8;
const int neptuneLED = 9;
const int plutoLED = 10; // 🆕 Pluto

// Define delay time (in milliseconds)
const int delayTime = 5000; // 5 seconds for each LED

void setup() {
  // Set LED pins as output
  pinMode(mercuryLED, OUTPUT);
  pinMode(venusLED, OUTPUT);
  pinMode(earthLED, OUTPUT);
  pinMode(marsLED, OUTPUT);
  pinMode(jupiterLED, OUTPUT);
  pinMode(saturnLED, OUTPUT);
  pinMode(uranusLED, OUTPUT);
  pinMode(neptuneLED, OUTPUT);
  pinMode(plutoLED, OUTPUT); // 🆕 Pluto
}

void loop() {
  // Light up LEDs one by one
  digitalWrite(mercuryLED, HIGH);
  delay(delayTime);
  digitalWrite(mercuryLED, LOW);

  digitalWrite(venusLED, HIGH);
  delay(delayTime);
  digitalWrite(venusLED, LOW);

  digitalWrite(earthLED, HIGH);
  delay(delayTime);
  digitalWrite(earthLED, LOW);

  digitalWrite(marsLED, HIGH);
  delay(delayTime);
  digitalWrite(marsLED, LOW);

  digitalWrite(jupiterLED, HIGH);
  delay(delayTime);
  digitalWrite(jupiterLED, LOW);

  digitalWrite(saturnLED, HIGH);
  delay(delayTime);
  digitalWrite(saturnLED, LOW);

  digitalWrite(uranusLED, HIGH);
  delay(delayTime);
  digitalWrite(uranusLED, LOW);

  digitalWrite(neptuneLED, HIGH);
  delay(delayTime);
  digitalWrite(neptuneLED, LOW);

  digitalWrite(plutoLED, HIGH);   // 🆕 Pluto
  delay(delayTime);
  digitalWrite(plutoLED, LOW);    // 🆕 Pluto

  // The sequence repeats
}
