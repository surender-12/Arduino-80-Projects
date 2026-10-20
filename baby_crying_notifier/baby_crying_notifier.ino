#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "BabyCryMonitor"
#define BLYNK_AUTH_TOKEN "LOEs8WnQp2edCGxsRjJiLRfqBLrcthvD"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi Credentials
char ssid[] = "Surender";
char pass[] = "1234567890";

const int soundPin = D5; // Sound sensor digital OUT
bool notified = false;

void setup() {
  Serial.begin(9600);
  pinMode(soundPin, INPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  int soundDetected = digitalRead(soundPin);

  if (soundDetected == HIGH && !notified) {
    Serial.println("Baby Cry Detected!");
    
    // Turn on virtual LED on Blynk app (V0)
    Blynk.virtualWrite(V0, 255); 

    // Send mobile notification
    Blynk.logEvent("baby_cry", "👶 Baby is crying!");

    notified = true;
    delay(10000); // avoid multiple notifications
  } 
  
  // Reset after a while
  if (soundDetected == LOW) {
    Blynk.virtualWrite(V0, 0);
    notified = false;
  }

  delay(100);
}
