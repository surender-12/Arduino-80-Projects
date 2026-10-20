#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi credentials
const char* ssid = "Surender";
const char* password = "12345678";

// Web server
ESP8266WebServer server(80);

// Pin definitions
#define SOIL_PIN A0
#define IN1 D1
#define IN2 D2

bool pumpState = false;

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected: " + WiFi.localIP().toString());

  // Web routes
  server.on("/", handleRoot);
  server.on("/ON", []() { pumpState = true; updatePump(); server.send(200, "text/plain", "OK"); });
  server.on("/OFF", []() { pumpState = false; updatePump(); server.send(200, "text/plain", "OK"); });
  server.on("/status", handleStatus);

  server.begin();
}

void loop() {
  server.handleClient();

  // Only read soil moisture for display
  delay(1000);
}

void updatePump() {
  if(pumpState){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}

void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Smart Irrigation</title>
    <style>
      body {
        font-family: Arial, sans-serif;
        text-align: center;
        background: linear-gradient(to bottom, #a8e063, #56ab2f);
        color: #fff;
      }
      h1 { font-size: 36px; margin-bottom: 20px; }
      button {
        padding: 15px 30px;
        font-size: 22px;
        margin: 10px;
        border: none;
        border-radius: 10px;
        cursor: pointer;
        background-color: #ff6b6b;
        color: white;
        transition: background 0.3s;
      }
      button:hover { background-color: #ff4757; }
      #motorStatus { font-size: 22px; margin-top: 20px; }
      #soilStatus { font-size: 22px; margin-top: 20px; }
      #soilBarContainer {
        width: 80%;
        background-color: #ddd;
        border-radius: 20px;
        margin: 10px auto 40px auto;
        height: 30px;
        overflow: hidden;
      }
      #soilBar {
        height: 100%;
        width: 0%;
        background: linear-gradient(to right, #00b09b, #96c93d);
        border-radius: 20px;
        transition: width 0.5s;
      }
    </style>
  </head>
  <body>
    <h1> Smart Irrigation </h1>
    <p>Motor Control:</p>
    <button onclick="togglePump('ON')">ON</button>
    <button onclick="togglePump('OFF')">OFF</button>
    <div id="motorStatus"></div> <!-- Motor status below buttons -->
    <div id="soilStatus"></div>  <!-- Soil moisture % -->
    <div id="soilBarContainer">
      <div id="soilBar"></div>
    </div>
    <script>
      function togglePump(action) {
        fetch('/' + action).then(() => updateStatus());
      }

      function updateStatus() {
        fetch('/status')
          .then(response => response.json())
          .then(data => {
            document.getElementById('motorStatus').innerHTML = 'Motor Status: ' + data.motor;
            document.getElementById('soilStatus').innerHTML = 'Soil Moisture: ' + data.soil + '%';
            document.getElementById('soilBar').style.width = data.soil + '%';
          });
      }

      setInterval(updateStatus, 1000);
      updateStatus();
    </script>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleStatus() {
  int soilValue = analogRead(SOIL_PIN);
  int soilPercent = map(soilValue, 1023, 0, 0, 100);
  String json = "{ \"soil\": " + String(soilPercent) + ", \"motor\": \"" + String(pumpState ? "ON" : "OFF") + "\" }";
  server.send(200, "application/json", json);
}
sMART IRRIGATION
