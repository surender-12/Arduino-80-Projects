#include <toneAC.h>   // If not available, use tone() instead of toneAC()

// Define pins for touch sensors
int touchPins[6] = {2, 3, 4, 5, 6, 7};  

// Frequencies for notes (Hz) – you can adjust these
int notes[6] = {262, 294, 330, 349, 392, 440};  
// C, D, E, F, G, A

int buzzerPin = 8;  // Buzzer connected here

void setup() {
  // Set all touch pins as INPUT
  for (int i = 0; i < 6; i++) {
    pinMode(touchPins[i], INPUT);
  }
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  for (int i = 0; i < 6; i++) {
    if (digitalRead(touchPins[i]) == HIGH) {   // Touch detected
      tone(buzzerPin, notes[i]);               // Play corresponding note
      delay(200);                              // Small delay so note is audible
      noTone(buzzerPin);                       // Stop sound
    }
  }
}
