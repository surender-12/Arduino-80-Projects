#include <Servo.h>

// ----- Pin configuration -----
const uint8_t FL_PIN   = 3;   // Front-Left leg servo
const uint8_t FR_PIN   = 5;   // Front-Right leg servo
const uint8_t RL_PIN   = 6;   // Rear-Left leg servo
const uint8_t RR_PIN   = 9;   // Rear-Right leg servo
const uint8_t HEAD_PIN = 10;  // Head servo (ultrasonic mounted)

const uint8_t TRIG_PIN = 11;  // HC-SR04 TRIG
const uint8_t ECHO_PIN = 12;  // HC-SR04 ECHO

// ----- Servos -----
Servo servoFL, servoFR, servoRL, servoRR, servoHead;

// ----- Gait/angle parameters (TUNE THESE for your mech) -----
// Standard neutral position for leg servos (support)
const int LEG_NEUTRAL = 90;

// When leg is lifted, move to this angle (represents "up")
const int LEG_LIFT = 60;      // smaller or larger depending on mech; try 60..100

// Forward swing angle (when swinging forward to take a step)
const int LEG_FORWARD = 120;  // try 110..140

// Backward/support angle (when leg is on ground pushing)
const int LEG_BACK = 70;      // try 60..90

// Head scan angles
const int HEAD_LEFT  = 40;
const int HEAD_CENTER = 90;
const int HEAD_RIGHT = 140;

// Movement timing (ms) - tune for speed/servo capability
const unsigned long STEP_DELAY = 180;   // delay between servo moves in a micro-step
const unsigned long STEP_PAUSE = 80;    // pause between phases
const unsigned long HEAD_SCAN_DELAY = 300; // pause while head moves

// Ultrasonic thresholds (cm)
const unsigned int OBSTACLE_STOP_DIST = 18;  // if object closer than this, stop / avoid
const unsigned int BACKUP_DISTANCE = 25;     // distance to back off to (not precise)

int stepCount = 0;

// ----- Helper: read ultrasonic distance (cm) -----
unsigned int readUltrasonicCM() {
  // Send 10us pulse to trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Read echo
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
  if (duration == 0) return 999; // no reading (out of range)
  unsigned int cm = duration / 29 / 2;
  return cm;
}

// ----- Basic leg movement helpers -----
void setLeg(Servo &s, int angle) {
  s.write(angle);
  delay(STEP_DELAY);
}

// Move a diagonal pair for a single step (simple trot)
void stepDiagonal(Servo &legA, Servo &legB,
                  int liftAngle, int forwardAngle, int backAngle) {
  // Phase 1: lift both legs
  setLeg(legA, liftAngle);
  setLeg(legB, liftAngle);
  delay(STEP_PAUSE);

  // Phase 2: swing forward
  setLeg(legA, forwardAngle);
  setLeg(legB, forwardAngle);
  delay(STEP_PAUSE);

  // Phase 3: lower to back/support
  setLeg(legA, backAngle);
  setLeg(legB, backAngle);
  delay(STEP_PAUSE);

  // Phase 4: return to neutral/support (helps ground contact)
  setLeg(legA, LEG_NEUTRAL);
  setLeg(legB, LEG_NEUTRAL);
  delay(STEP_PAUSE);
}

// Simple backup maneuver - all legs move to produce small backward motion
void backupRoutine() {
  // Raise back legs slightly and swing them backward, front swing forward
  setLeg(servoRL, LEG_LIFT);
  setLeg(servoRR, LEG_LIFT);
  delay(STEP_DELAY);

  setLeg(servoRL, LEG_BACK);
  setLeg(servoRR, LEG_BACK);
  delay(200);

  // small forward push with front legs
  setLeg(servoFL, LEG_FORWARD);
  setLeg(servoFR, LEG_FORWARD);
  delay(200);

  // Reset to neutral
  setLeg(servoFL, LEG_NEUTRAL);
  setLeg(servoFR, LEG_NEUTRAL);
  setLeg(servoRL, LEG_NEUTRAL);
  setLeg(servoRR, LEG_NEUTRAL);
  delay(200);
}

// Head scan and ultrasonic check
unsigned int headScanAndCheck() {
  // Look left
  servoHead.write(HEAD_LEFT);
  delay(HEAD_SCAN_DELAY);
  unsigned int dLeft = readUltrasonicCM();

  // Center
  servoHead.write(HEAD_CENTER);
  delay(HEAD_SCAN_DELAY);
  unsigned int dCenter = readUltrasonicCM();

  // Look right
  servoHead.write(HEAD_RIGHT);
  delay(HEAD_SCAN_DELAY);
  unsigned int dRight = readUltrasonicCM();

  // Return center
  servoHead.write(HEAD_CENTER);
  delay(HEAD_SCAN_DELAY);

  // Choose the minimum distance measured
  unsigned int minD = min(dLeft, min(dCenter, dRight));
  return minD;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Walking robot starting...");

  // Attach servos
  servoFL.attach(FL_PIN);
  servoFR.attach(FR_PIN);
  servoRL.attach(RL_PIN);
  servoRR.attach(RR_PIN);
  servoHead.attach(HEAD_PIN);

  // Ultrasonic pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Move all servos to neutral starting pose
  servoFL.write(LEG_NEUTRAL);
  servoFR.write(LEG_NEUTRAL);
  servoRL.write(LEG_NEUTRAL);
  servoRR.write(LEG_NEUTRAL);
  servoHead.write(HEAD_CENTER);

  delay(500);
}

void loop() {
  // Read center distance to quickly check environment
  unsigned int dist = readUltrasonicCM();
  if (dist < OBSTACLE_STOP_DIST) {
    Serial.print("Obstacle detected at ");
    Serial.print(dist);
    Serial.println(" cm. Backing up...");
    backupRoutine();
    delay(300);
    // After backing, re-check distance
    unsigned int d2 = readUltrasonicCM();
    if (d2 < OBSTACLE_STOP_DIST) {
      // If still too close, rotate head to find clearer path
      Serial.println("Still close after backup — scanning head.");
      unsigned int dscan = headScanAndCheck();
      Serial.print("Scan min distance: ");
      Serial.println(dscan);
      // If path still blocked, stop here (could implement rotation)
      if (dscan < OBSTACLE_STOP_DIST) {
        Serial.println("Path blocked. Stopping.");
        // Hold neutral pose
        while (true) {
          delay(500);
        }
      }
    }
  }

  // Perform a step with diagonal pair 1: FrontLeft + RearRight
  stepDiagonal(servoFL, servoRR, LEG_LIFT, LEG_FORWARD, LEG_BACK);
  stepCount++;

  // After 3 steps rotate head (scan) as requested
  if (stepCount % 3 == 0) {
    unsigned int minD = headScanAndCheck();
    Serial.print("Head scan min distance: ");
    Serial.println(minD);
    // If something too close, do a small backup
    if (minD < OBSTACLE_STOP_DIST) {
      backupRoutine();
    }
  }

  // Perform a step with diagonal pair 2: FrontRight + RearLeft
  stepDiagonal(servoFR, servoRL, LEG_LIFT, LEG_FORWARD, LEG_BACK);
  stepCount++;

  // Small pause before next cycle
  delay(120);
}
