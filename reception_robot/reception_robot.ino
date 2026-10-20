#include <Servo.h>

Servo handServo;  // Servo for waving hand
Servo headServo;  // Servo for head rotation

const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

void setup() {
    Serial.begin(9600);
    
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    
    handServo.attach(6);  // Hand servo on pin 6
    headServo.attach(5);  // Head servo on pin 5
    
    handServo.write(180);  // Initial hand position (raised)
    headServo.write(90);   // Initial head position (centered)
}

void loop() {
    // Measure distance using ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance > 0 && distance <= 50) { // If person detected within 50cm
        moveHand();
        moveHead();
    }

    delay(500);
}

void moveHand() {
    // Move hand from 180° to 0° and back to 180°
    for (int pos = 180; pos >= 0; pos -= 10) { 
        handServo.write(pos);
        delay(100);
    }
    delay(200);
    for (int pos = 0; pos <= 180; pos += 10) { 
        handServo.write(pos);
        delay(100);
    }
}

void moveHead() {
    // Move head from 90° to 0°, then back to 90°, then to 180°
    for (int pos = 90; pos >= 0; pos -= 10) { 
        headServo.write(pos);
        delay(100);
    }
    delay(200);
    for (int pos = 0; pos <= 90; pos += 10) { 
        headServo.write(pos);
        delay(100);
    }
    delay(200);
    for (int pos = 90; pos <= 180; pos += 10) { 
        headServo.write(pos);
        delay(100);
    }
}
