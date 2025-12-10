#include <Servo.h>

// ------------------ SERVO SETUP ------------------
Servo hip;
Servo knee;

int hipPin = 9;
int kneePin = 10;

// ------------------ LEG GEOMETRY ------------------
// Your real measured link lengths (mm)
float L1 = 140;   // hip → knee
float L2 = 120;   // knee → foot

// ------------------ INVERSE KINEMATICS ------------------
void moveToXY(float x, float y) {

  // IK math
  float D = (x*x + y*y - L1*L1 - L2*L2) / (2 * L1 * L2);
  D = constrain(D, -1.0, 1.0);   // avoid NaN

  float kneeRad = acos(D);
  float kneeDeg = kneeRad * 180.0 / PI;

  float hipRad = atan2(y, x) - atan2(L2 * sin(kneeRad), L1 + L2 * cos(kneeRad));
  float hipDeg = hipRad * 180.0 / PI;

  // Adjust depending on physical orientation
  float hipServo  = hipDeg + 90;
  float kneeServo = 180 - kneeDeg;

  hip.write(hipServo);
  knee.write(kneeServo);
}

// ------------------ CIRCLE MOTION ------------------
void moveCircle(float cx, float cy, float radius, int steps, bool clockwise, int stepDelayMs) {
  for (int i = 0; i <= steps; i++) {

    float t = (float)i / (float)steps;
    float angle = clockwise ? (2 * PI * (1 - t)) : (2 * PI * t);

    float x = cx + radius * cos(angle);
    float y = cy + radius * sin(angle);

    moveToXY(x, y);
    delay(stepDelayMs);
  }
}

// ------------------ SETUP ------------------
void setup() {
  hip.attach(hipPin);
  knee.attach(kneePin);

  // Strong alignment pulse
  hip.write(90);
  knee.write(90);
  delay(1500);

  // Move to safe neutral point
  moveToXY(120, -150);
  delay(700);
}

// ------------------ LOOP ------------------
void loop() {

  float cx = 140;     // center X
  float cy = -150;    // center Y
  float radius = 20;  // circle radius
  int steps = 150;    // smooth
  int d = 5;          // fast motion

  moveCircle(cx, cy, radius, steps, true, d);

  delay(200);
}
