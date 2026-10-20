#include <Keypad.h>
#include <Servo.h>

// -------- Servo ----------
Servo myServo;
int servoPin = 10;

// -------- Password ----------
String password = "1234";   // CHANGE YOUR PASSWORD HERE
String input = "";

// -------- Keypad ----------
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  myServo.write(0);   // LOCK position
  Serial.println("Enter 4-digit password:");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print(key);

    if (key == '*') {          // Reset input
      input = "";
      Serial.println("\nCleared");
    }
    else if (key == '#') {     // Enter key
      checkPassword();
      input = "";
    }
    else {
      input += key;            // Add digit
    }
  }
}

void checkPassword() {
  if (input == password) {
    Serial.println("\nAccess Granted");
    myServo.write(90);   // OPEN
    delay(3000);         // Keep open
    myServo.write(0);    // LOCK again
  } else {
    Serial.println("\nWrong Password");
  }
}