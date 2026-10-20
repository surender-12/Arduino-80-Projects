#include <Keypad.h>
#include <Servo.h>

// Define the servo motor
Servo myServo;
const int servoPin = 10;  // Servo control pin

// Define keypad size
const byte ROWS = 4; 
const byte COLS = 4; 

// Define keypad layout
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Define keypad pins
byte rowPins[ROWS] = {2, 3, 4, 5}; // Row pins
byte colPins[COLS] = {6, 7, 8, 9}; // Column pins

// Create Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Set password
String password = "1234";  
String inputPassword = "";  

void setup() {
    myServo.attach(servoPin);
    myServo.write(0);  // Initial locked position
    Serial.begin(9600);
}

void loop() {
    char key = keypad.getKey();  // Read keypad input

    if (key) {  // If a key is pressed
        Serial.print("Entered: "); Serial.println(key);
        inputPassword += key;  

        if (inputPassword.length() == 4) {  // If 4 digits are entered
            if (inputPassword == password) {
                Serial.println("✅ Password Correct! Unlocking...");
                myServo.write(90);  // Move servo to unlock position
                delay(3000);  // Keep unlocked for 3 sec
                myServo.write(0);   // Lock again
            } else {
                Serial.println("❌ Incorrect Password! Try Again.");
            }
            inputPassword = "";  // Reset input
        }
    }
}
