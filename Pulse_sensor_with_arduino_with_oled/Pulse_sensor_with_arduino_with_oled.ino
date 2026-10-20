#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PulseSensorPlayground.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int PulseSensorPin = A0;
const int Threshold = 550;
PulseSensorPlayground pulseSensor;

int ecgX = 0;

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Starting...");
  display.display();

  pulseSensor.analogInput(PulseSensorPin);
  pulseSensor.setThreshold(Threshold);
  pulseSensor.begin();
}

void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute();
  int signal = analogRead(PulseSensorPin);
  int y = map(signal, 500, 1023, 63, 20);

  // Draw the ECG waveform
  display.drawPixel(ecgX, y, SSD1306_WHITE);

  // Bigger BPM Display
  display.fillRect(0, 0, 128, 20, SSD1306_BLACK); // Clear top area
  display.setTextSize(2);                         // 👈 Larger font
  display.setCursor(0, 0);
  display.print("BPM:");
  display.setCursor(70, 0);
  display.print(myBPM);

  display.display();

  ecgX++;
  if (ecgX >= SCREEN_WIDTH) {
    ecgX = 0;
    display.clearDisplay();
  }

  delay(20);
}
