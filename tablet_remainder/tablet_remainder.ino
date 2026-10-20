#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

const int LED1 = 8; // Morning LED
const int LED2 = 9; // Noon LED
const int LED3 = 10; // Night LED
const int buzzer = 11;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    rtc.begin();

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(buzzer, OUTPUT);
}

void loop() {
    DateTime now = rtc.now();
    int hour = now.hour();
    int minute = now.minute();

    // Print RTC time for debugging
    Serial.print("Time: ");
    Serial.print(hour);
    Serial.print(":");
    Serial.println(minute);

    // Reset LEDs and buzzer
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(buzzer, LOW);

    // Check for alarm times
    if (hour == 13 && minute == 33) {
        reminder(LED1); // 16:35 - First LED
    }
    else if (hour == 13 && minute == 34) {
        reminder(LED2); // 16:36 - Second LED
    }
    else if (hour == 13 && minute == 35) {
        reminder(LED3); // 16:37 - Third LED
    }

    delay(10000); // Check time every 10 seconds
}

void reminder(int ledPin) {
    digitalWrite(ledPin, HIGH);
    for (int i = 0; i < 5; i++) { // Beep 5 times
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);
        delay(500);
    }
}
