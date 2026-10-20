// ESP8266 -> MPU6050 -> WebSocket (with embedded function-style MadgwickAHRS)
// This sketch embeds a MadgwickAHRSupdate(...) implementation and q0..q3 globals
// so you won't get "MadgwickAHRSupdate not declared" errors.
// Libraries needed: ESP8266WiFi, WebSocketsServer, Wire, MPU6050 (Jeff Rowberg)

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Wire.h>
#include "MPU6050.h"

// -------------------- Embedded minimal Madgwick (function-style) --------------------
// This is a compact implementation of Madgwick's AHRS algorithm (function-style).
// It defines: void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az);
// and global floats q0,q1,q2,q3. Units: gx,gy,gz in rad/s; ax,ay,az in g.

float beta = 0.05f; // 2 * proportional gain (tune 0.01..0.2)
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;

void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az) {
  // Reference implementation (simplified, 6-DOF)
  float recipNorm;
  float s0, s1, s2, s3;
  float qDot1, qDot2, qDot3, qDot4;
  float _2q0 = 2.0f * q0;
  float _2q1 = 2.0f * q1;
  float _2q2 = 2.0f * q2;
  float _2q3 = 2.0f * q3;
  float _4q0 = 4.0f * q0;
  float _4q1 = 4.0f * q1;
  float _4q2 = 4.0f * q2;
  float _8q1 = 8.0f * q1;
  float _8q2 = 8.0f * q2;
  float q0q0 = q0 * q0;
  float q1q1 = q1 * q1;
  float q2q2 = q2 * q2;
  float q3q3 = q3 * q3;

  // Rate of change of quaternion from gyroscope
  qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
  qDot2 = 0.5f * ( q0 * gx + q2 * gz - q3 * gy);
  qDot3 = 0.5f * ( q0 * gy - q1 * gz + q3 * gx);
  qDot4 = 0.5f * ( q0 * gz + q1 * gy - q2 * gx);

  // Compute feedback only if accelerometer measurement valid (avoids NaN)
  if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
    // Normalize accelerometer measurement
    recipNorm = sqrtf(ax * ax + ay * ay + az * az);
    if (recipNorm == 0.0f) return;
    recipNorm = 1.0f / recipNorm;
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;

    // Auxiliary variables to avoid repeated arithmetic
    float _2q0q2 = 2.0f * q0 * q2;
    float _2q2q3 = 2.0f * q2 * q3;

    // Gradient decent algorithm corrective step
    s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
    s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
    s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
    s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;

    // Normalize step magnitude
    recipNorm = sqrtf(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
    if (recipNorm == 0.0f) return;
    recipNorm = 1.0f / recipNorm;
    s0 *= recipNorm;
    s1 *= recipNorm;
    s2 *= recipNorm;
    s3 *= recipNorm;

    // Apply feedback step
    qDot1 -= beta * s0;
    qDot2 -= beta * s1;
    qDot3 -= beta * s2;
    qDot4 -= beta * s3;
  }

  // Integrate rate of change to yield quaternion
  q0 += qDot1 * (1.0f / 100.0f); // dt will be supplied by external sample rate; we use nominal 100Hz here
  q1 += qDot2 * (1.0f / 100.0f);
  q2 += qDot3 * (1.0f / 100.0f);
  q3 += qDot4 * (1.0f / 100.0f);

  // Normalize quaternion
  recipNorm = sqrtf(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
  if (recipNorm == 0.0f) return;
  recipNorm = 1.0f / recipNorm;
  q0 *= recipNorm;
  q1 *= recipNorm;
  q2 *= recipNorm;
  q3 *= recipNorm;
}
// -------------------- End Madgwick embed --------------------


MPU6050 mpu;
WebSocketsServer webSocket = WebSocketsServer(81);

const char *apSSID = "ESP_AirCanvas";
const char *apPassword = "air12345";

const float SAMPLE_HZ = 100.0;
const unsigned long SAMPLE_US = (unsigned long)(1000000.0 / SAMPLE_HZ);

const float GYRO_SCALE = 131.0;    // LSB/(deg/s) for gyro FS=±250dps
const float ACCEL_SCALE = 16384.0; // LSB/g for accel FS=±2g
const float DEG2RAD = 3.14159265358979323846 / 180.0;

float gyroBiasX=0, gyroBiasY=0, gyroBiasZ=0;
float accelBiasX=0, accelBiasY=0, accelBiasZ=0;

unsigned long lastMicros = 0;
bool clientsConnected = false;

// -------------------- Forward declarations (prototypes) --------------------
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void sendFrame(float q0, float q1, float q2, float q3,
               float gx, float gy, float gz,
               float ax, float ay, float az);
void calibrateSensors(int samples);
// -------------------- End prototypes --------------------

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\nESP AirCanvas (embedded Madgwick) starting...");

  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU connection failed!");
    while (1) delay(1000);
  }

  WiFi.softAP(apSSID, apPassword);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  calibrateSensors(200); // keep device still

  lastMicros = micros();
}

void loop() {
  webSocket.loop();

  unsigned long now = micros();
  unsigned long dtUs = now - lastMicros;
  if (dtUs < SAMPLE_US) return;
  float dt = dtUs / 1e6;
  lastMicros = now;

  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Convert to physical units
  float ax_f = (float)ax / ACCEL_SCALE; // in g
  float ay_f = (float)ay / ACCEL_SCALE;
  float az_f = (float)az / ACCEL_SCALE;
  float gx_dps = (float)gx / GYRO_SCALE; // deg/s
  float gy_dps = (float)gy / GYRO_SCALE;
  float gz_dps = (float)gz / GYRO_SCALE;

  // Remove biases
  gx_dps -= gyroBiasX;
  gy_dps -= gyroBiasY;
  gz_dps -= gyroBiasZ;

  ax_f -= accelBiasX;
  ay_f -= accelBiasY;
  az_f -= accelBiasZ;

  // Convert gyro to rad/s for Madgwick
  float gx_r = gx_dps * DEG2RAD;
  float gy_r = gy_dps * DEG2RAD;
  float gz_r = gz_dps * DEG2RAD;

  // Update embedded MadgwickAHRS (units: rad/s, g)
  MadgwickAHRSupdate(gx_r, gy_r, gz_r, ax_f, ay_f, az_f);

  // q0..q3 are global
  sendFrame(q0, q1, q2, q3, gx_dps, gy_dps, gz_dps, ax_f, ay_f, az_f);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_CONNECTED) {
    clientsConnected = true;
    IPAddress ip = webSocket.remoteIP(num);
    Serial.printf("Client %u connected: %s\n", num, ip.toString().c_str());
    webSocket.sendTXT(num, "{\"type\":\"hello\",\"msg\":\"ESP AirCanvas connected\"}");
  } else if (type == WStype_DISCONNECTED) {
    Serial.printf("Client %u disconnected\n", num);
    clientsConnected = (webSocket.connectedClients() > 0);
  } else if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    if (msg == "calibrate") {
      calibrateSensors(200);
      webSocket.broadcastTXT("{\"type\":\"calibrated\"}");
    }
  }
}

void sendFrame(float q0, float q1, float q2, float q3,
               float gx, float gy, float gz,
               float ax, float ay, float az)
{
  unsigned long t = millis();
  char buf[220];
  int n = snprintf(buf, sizeof(buf),
      "{\"t\":%lu,\"q\":[%.6f,%.6f,%.6f,%.6f],\"g\":[%.3f,%.3f,%.3f],\"a\":[%.3f,%.3f,%.3f]}",
      t, q0, q1, q2, q3, gx, gy, gz, ax, ay, az);
  if (n > 0) {
    webSocket.broadcastTXT(buf);
  }
}

void calibrateSensors(int samples) {
  Serial.println("Calibrating... keep device still.");
  long gxSum=0, gySum=0, gzSum=0;
  long axSum=0, aySum=0, azSum=0;
  for (int i=0;i<samples;i++) {
    int16_t axr, ayr, azr, gxr, gyr, gzr;
    mpu.getMotion6(&axr,&ayr,&azr,&gxr,&gyr,&gzr);
    gxSum += gxr; gySum += gyr; gzSum += gzr;
    axSum += axr; aySum += ayr; azSum += azr;
    delay(5);
  }
  float gxAvg = (float)gxSum / samples;
  float gyAvg = (float)gySum / samples;
  float gzAvg = (float)gzSum / samples;
  float axAvg = (float)axSum / samples;
  float ayAvg = (float)aySum / samples;
  float azAvg = (float)azSum / samples;

  gyroBiasX = gxAvg / GYRO_SCALE;
  gyroBiasY = gyAvg / GYRO_SCALE;
  gyroBiasZ = gzAvg / GYRO_SCALE;

  accelBiasX = axAvg / ACCEL_SCALE;
  accelBiasY = ayAvg / ACCEL_SCALE;
  accelBiasZ = (azAvg / ACCEL_SCALE) - 1.0; // gravity on Z

  Serial.println("Calibration done:");
  Serial.printf("gyro biases (dps): %.3f, %.3f, %.3f\n", gyroBiasX, gyroBiasY, gyroBiasZ);
  Serial.printf("acc biases (g): %.4f, %.4f, %.4f\n", accelBiasX, accelBiasY, accelBiasZ);
}
