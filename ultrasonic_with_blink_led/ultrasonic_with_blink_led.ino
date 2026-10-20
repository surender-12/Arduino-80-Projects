#include <ESP8266WiFi.h>

// Set up the LED pin (GPIO5 on D1 pin)
const int ledPin = D1;

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);

  // Connect to WiFi
  WiFi.begin("saikumari", "1234567890");

  // Wait until connected to WiFi
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Turn the LED on
  digitalWrite(ledPin, HIGH);
  delay(1000); // Wait for 1 second

  // Turn the LED off
  digitalWrite(ledPin, LOW);
  delay(1000); // Wait for 1 second
}
