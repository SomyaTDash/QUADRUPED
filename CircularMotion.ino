#include <Servo.h>
#include <math.h>

Servo hipServo;
Servo kneeServo;

// ---------------- LEG DIMENSIONS (cm) ----------------
float L1 = 14.0;
float L2 = 12.0;

// ---------------- SERVO PINS ----------------
int hipPin = 9;
int kneePin = 10;

// ---------------- CIRCLE PARAMETERS ----------------
float cx = 10.0;
float cy = -10.0;
float r  = 3.0;

// ---------------- INITIAL SERVO POSITIONS ----------------
// Adjust these to match your mechanical neutral pose
int hipInit  = 90;
int kneeInit = 90;

void setup() {
  hipServo.attach(hipPin);
  kneeServo.attach(kneePin);

  Serial.begin(9600);

  // -------- INITIALIZE LEG POSITION --------
  initializeLeg();
}

void loop() {
  drawCircle();
}

// ------------------------------------------------------
// FUNCTION: Smooth initialization
// ------------------------------------------------------
void initializeLeg() {
  int currentHip  = hipServo.read();
  int currentKnee = kneeServo.read();

  for (int i = 0; i <= 50; i++) {
    int hipPos  = map(i, 0, 50, currentHip, hipInit);
    int kneePos = map(i, 0, 50, currentKnee, kneeInit);

    hipServo.write(hipPos);
    kneeServo.write(kneePos);

    delay(20);
  }

  delay(1000); // settle time
}

// ------------------------------------------------------
// FUNCTION: Draw circle using IK
// ------------------------------------------------------
void drawCircle() {
  for (int deg = 0; deg <= 360; deg += 5) {

    float t = radians(deg);

    // Circle point
    float x = cx + r * cos(t);
    float y = cy + r * sin(t);

    float d = sqrt(x * x + y * y);
    if (d > (L1 + L2)) continue;

    // -------- IK --------
    float cosKnee = (L1*L1 + L2*L2 - d*d) / (2 * L1 * L2);
    float kneeRad = acos(constrain(cosKnee, -1, 1));

    float alpha = atan2(y, x);
    float beta  = acos((L1*L1 + d*d - L2*L2) / (2 * L1 * d));
    float hipRad = alpha - beta;

    float hipDeg  = degrees(hipRad);
    float kneeDeg = degrees(kneeRad);

    // -------- Servo mapping --------
    int hipServoAngle  = 90 + hipDeg;
    int kneeServoAngle = 180 - kneeDeg;

    hipServo.write(constrain(hipServoAngle, 0, 180));
    kneeServo.write(constrain(kneeServoAngle, 0, 180));

    delay(30);
  }
}

