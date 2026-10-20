// TA6586 D0–D3 test

#define D0 19
#define D1 21
#define D2 22
#define D3 23

void setup() {
  ledcAttach(D0, 20000, 8);
  ledcAttach(D1, 20000, 8);
  ledcAttach(D2, 20000, 8);
  ledcAttach(D3, 20000, 8);
}

void loop() {
  // FORWARD
  ledcWrite(D0, 150);  // Motor A forward
  ledcWrite(D1, 0);
  ledcWrite(D2, 150);  // Motor B forward
  ledcWrite(D3, 0);
  delay(3000);

  // STOP
  ledcWrite(D0, 0);
  ledcWrite(D1, 0);
  ledcWrite(D2, 0);
  ledcWrite(D3, 0);
  delay(2000);

  // BACKWARD
  ledcWrite(D0, 0);
  ledcWrite(D1, 150);  // Motor A reverse
  ledcWrite(D2, 0);
  ledcWrite(D3, 150);  // Motor B reverse
  delay(3000);
}
