/****************************************************
 GPS SCHOOL BELT – ESP8266 (NodeMCU)
 GPS: NEO-6M
 Library: TinyGPS (neosarchizo)
 Mode: WiFi Router (STA)
****************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

/* -------- WIFI -------- */
const char* ssid = "IDPS300";
const char* password = "IDPS@123";

/* -------- GPS -------- */
static const int RXPin = D6;
static const int TXPin = D5;
static const uint32_t GPSBaud = 9600;

TinyGPS gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

/* -------- SERVER -------- */
ESP8266WebServer server(80);

/* -------- GPS DATA -------- */
float latitude = 0.0;
float longitude = 0.0;
bool gpsFix = false;

/* -------- HTML -------- */
const char MAIN_page[] PROGMEM = R"====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>GPS School Belt</title>

<style>
body{margin:0;font-family:Arial;background:#eef2f5}
.header{
background:#0d47a1;color:white;
padding:14px;text-align:center;
font-size:20px;font-weight:bold
}
.card{
background:white;margin:10px;
padding:14px;border-radius:10px;
box-shadow:0 4px 10px rgba(0,0,0,.12)
}
#map{
width:100%;height:60vh;
border:0;border-radius:12px
}
.footer{
text-align:center;
padding:10px;
font-size:14px;
font-weight:bold;
color:#0d47a1
}
.badge{color:green;font-size:13px}
</style>
</head>

<body>

<div class="header">IDPS SCHOOL</div>

<div class="card">
<h3>Live Location <span class="badge" id="badge"></span></h3>
<iframe id="map"></iframe>
</div>

<div class="footer">
📍 International Delhi Public School, Auto Sai Nagar Colony, Vanasthalipuram
</div>

<script>
const PLACE_QUERY =
"International+Delhi+Public+School+Auto+Sai+Nagar+Colony+Vanasthalipuram+Hyderabad";

function showPlace(){
document.getElementById("map").src =
"https://www.google.com/maps?q=" + PLACE_QUERY + "&z=17&output=embed";
}

function showLive(lat, lon){
document.getElementById("map").src =
"https://www.google.com/maps?q=" + lat + "," + lon + "&z=17&output=embed";
}

showPlace();

setInterval(()=>{
fetch("/gps")
.then(r=>r.json())
.then(d=>{
if(d.fix){
showLive(d.lat, d.lon);
document.getElementById("badge").innerHTML = "● LIVE";
}
});
},5000);
</script>

</body>
</html>
)====";

/* -------- ROUTES -------- */
void handleRoot(){
  server.send_P(200,"text/html",MAIN_page);
}

void handleGPS(){
  String json="{";
  json+="\"fix\":";
  json+=gpsFix?"true":"false";
  json+=",\"lat\":";
  json+=String(latitude,6);
  json+=",\"lon\":";
  json+=String(longitude,6);
  json+="}";
  server.send(200,"application/json",json);
}

/* -------- SETUP -------- */
void setup(){
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);

  Serial.println();
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("Open this in browser: http://");
  Serial.println(WiFi.localIP());

  server.on("/",handleRoot);
  server.on("/gps",handleGPS);
  server.begin();

  Serial.println("Web server started");
}

/* -------- LOOP -------- */
void loop(){
  server.handleClient();

  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    if (gps.encode(c)) {
      unsigned long age;
      gps.f_get_position(&latitude, &longitude, &age);

      if (age < 5000 && latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        gpsFix = true;
      } else {
        gpsFix = false;
      }
    }
  }
}
