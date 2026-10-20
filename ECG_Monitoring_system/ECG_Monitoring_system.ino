#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define WIFISSID "Surender"
#define PASSWORD "1234567890"

#define WRITE_API_KEY "X5TBQL5ZMHNSW4LT"  // Replace with your ThingSpeak Write API Key
#define SENSOR A0

WiFiClient client;  // Required for HTTPClient now

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR, INPUT);

  WiFi.begin(WIFISSID, PASSWORD);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float ecgValue = analogRead(SENSOR);
    Serial.print("ECG Value: ");
    Serial.println(ecgValue);

    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + String(WRITE_API_KEY) + "&field1=" + String(ecgValue);

    http.begin(client, url);  // ✅ Updated: Pass WiFiClient and URL
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.println("Data sent to ThingSpeak");
    } else {
      Serial.println("Failed to send data, error: " + String(httpCode));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }

  delay(1000);  // Optional: Increase to 15 seconds to match ThingSpeak free rate limit
}
