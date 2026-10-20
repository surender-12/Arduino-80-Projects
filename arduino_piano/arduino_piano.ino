const int buzzerPin = 8;

// Touch sensor pins
const int touchPins[6] = {2, 3, 4, 5, 6, 7};

// Notes for 6 piano keys (C4 to A4)
const int notes[6] = {
  262, // C4
  294, // D4
  330, // E4
  349, // F4
  392, // G4
  440  // A4
};

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(touchPins[i], INPUT);
  }
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  for (int i = 0; i < 6; i++) {
    if (digitalRead(touchPins[i]) == HIGH) {
      tone(buzzerPin, notes[i], 200);  // Play note for 200 ms
      delay(220);                      // Short delay to avoid overlap
    }
  }
}
