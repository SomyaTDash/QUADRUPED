#include <Servo.h>

Servo hip, knee;

int hipPin = 9;
int kneePin = 10;

float L1 = 50;
float L2 = 50;


// FAST IK --------------------------------------------------
void moveToXY(float x, float y) {

  float D = (x*x + y*y - L1*L1 - L2*L2) / (2 * L1 * L2);
  D = constrain(D, -1.0, 1.0);

  float kneeRad = acos(D);
  float kneeDeg = kneeRad * 180.0 / PI;

  float s2 = sin(kneeRad);
  float c2 = cos(kneeRad);

  float k1 = L1 + L2 * c2;
  float k2 = L2 * s2;

  float hipRad = atan2(y, x) - atan2(k2, k1);
  float hipDeg = hipRad * 180.0 / PI;

  hip.write(hipDeg + 90);
  knee.write(180 - kneeDeg);
}


// FASTER CIRCLE -------------------------------------------
void moveCircle(float cx, float cy, float r, int steps, bool cw, int delayMS) {

  float dTheta = 2 * PI / steps;

  for (int i = 0; i < steps; i++) {

    float angle = cw ? (2*PI - i*dTheta) : (i*dTheta);

    float x = cx + r * cos(angle);
    float y = cy + r * sin(angle);

    moveToXY(x, y);
    delay(delayMS);
  }
}


void setup() {
  hip.attach(hipPin);
  knee.attach(kneePin);

  hip.write(90);
  knee.write(90);
  delay(800);

  moveToXY(60, -80);
  delay(400);
}


void loop() {

  moveCircle(70, -80, 15, 60, true, 5);

  delay(200);
}
