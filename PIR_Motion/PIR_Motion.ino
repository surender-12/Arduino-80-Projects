const uint8_t PIR_PIN = 2;      
const uint8_t BUZZER_PIN = 11;  
const uint8_t LED_PIN = 13;     

const unsigned long holdTimeMs = 2000; 


const bool useTone = false;
const unsigned int toneFreqHz = 2000;

unsigned long lastMotionMillis = 0;
bool buzzerOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);         
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println("PIR -> Buzzer ready");
  Serial.println("Watching for motion on pin D2. Buzzer on D11.");
}

void loop() {
  int pir = digitalRead(PIR_PIN);

  if (pir == HIGH) {
    // motion detected -> record time and ensure buzzer on
    lastMotionMillis = millis();
    if (!buzzerOn) {
      startBuzzer();
    }
  }

  // if within hold time after last motion, keep buzzer on
  if (millis() - lastMotionMillis <= holdTimeMs) {
    // keep buzzer on (already handled)
  } else {
    // no recent motion -> turn buzzer off if it was on
    if (buzzerOn) {
      stopBuzzer();
    }
  }

  // Debug prints every ~500 ms (non-blocking)
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    Serial.print("PIR=");
    Serial.print(pir);
    Serial.print("  buzzerOn=");
    Serial.println(buzzerOn ? "YES" : "NO");
  }
}

// Turn buzzer ON
void startBuzzer() {
  buzzerOn = true;
  digitalWrite(LED_PIN, HIGH);
  if (useTone) {
    tone(BUZZER_PIN, toneFreqHz);
  } else {
    digitalWrite(BUZZER_PIN, HIGH); // active buzzer
  }
}

// Turn buzzer OFF
void stopBuzzer() {
  buzzerOn = false;
  digitalWrite(LED_PIN, LOW);
  if (useTone) {
    noTone(BUZZER_PIN);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}
