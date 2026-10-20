#include <Keypad.h>
#include <Servo.h>

// Keypad setup
const byte ROW_NUM    = 4; // four rows
const byte COL_NUM    = 4; // four columns

char keys[ROW_NUM][COL_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pin_rows[ROW_NUM] = {2, 3, 4, 5};   // connect to the row pinouts of the keypad
byte pin_column[COL_NUM] = {6, 7, 8, 9}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COL_NUM);

// Servo motor setup
Servo myServo;
int servoPin = 10; // Servo signal connected to pin 10
String inputPassword = "";
String correctPassword = "1234"; // Change this to your desired password

void setup() {
  Serial.begin(9600);  // Start the serial monitor for debugging
  myServo.attach(servoPin);  // Attach the servo to the pin
  myServo.write(0);  // Set the initial position of the servo
}

void loop() {
  char key = keypad.getKey();  // Get the pressed key from the keypad

  if (key) {
    Serial.println(key);  // Print the key for debugging
    inputPassword += key;  // Add the pressed key to the inputPassword string

    // Check if inputPassword matches the correct password
    if (inputPassword.length() == 4) {
      if (inputPassword == correctPassword) {
        Serial.println("Password Correct!");
        myServo.write(90);  // Move servo to 90 degrees (open position)
      } else {
        Serial.println("Incorrect Password");
      }
      delay(2000);  // Wait for 2 seconds
      inputPassword = "";  // Clear the entered password
      myServo.write(0);  // Reset servo position to 0 degrees (closed position)
    }
  }
}
