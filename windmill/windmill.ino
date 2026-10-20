#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ENA 9  // PWM pin for speed control
#define IN1 7
#define IN2 8

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD address

int slowPWM = 100;   // Slow speed (adjustable)
int mediumPWM = 180; // Medium speed (adjustable)
float maxRPM = 200;  // Estimated max RPM of motor

void setup() {
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    lcd.begin();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Speed: 0 RPM");

    Serial.begin(9600);
}

void loop() {
    // Step 1: Slow Speed (30 seconds)
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, slowPWM);
    displaySpeed(slowPWM);
    delay(3000);

    // Step 2: Medium Speed (30 seconds)
    analogWrite(ENA, mediumPWM);
    displaySpeed(mediumPWM);
    delay(3000);

    // Step 3: Motor OFF (5 seconds)
    analogWrite(ENA, 0);
    lcd.setCursor(0, 0);
    lcd.print("Speed: 0 RPM  ");
    Serial.println("Motor OFF");
    delay(3000);
}

// Function to estimate and display speed
void displaySpeed(int pwmValue) {
    float rpm = (pwmValue / 255.0) * maxRPM; // Estimate speed
    lcd.setCursor(0, 0);
    lcd.print("Speed: ");
    lcd.setCursor(7, 0);
    lcd.print(rpm);
    lcd.print(" RPM  ");

    Serial.print("Motor Speed: ");
    Serial.print(rpm);
    Serial.println(" RPM");
}
