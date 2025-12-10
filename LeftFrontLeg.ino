#include <Servo.h>

// ------- SERVO SETUP -------
Servo hip;    // hip joint (forward/back)
Servo knee;   // knee joint (up/down)

// choose your pins
int hipPin  = 9;     // hip servo PWM pin
int kneePin = 10;    // knee servo PWM pin

// ------- SERVO CALIBRATION -------
int hipZero  = 90;   // mechanical zero for hip
int kneeZero = 90;   // mechanical zero for knee
int hipDir   = 1;    // direction multiplier
int kneeDir  = 1;

// ------- LEG GEOMETRY (mm) -------
float L1 = 50.0;     // upper leg
float L2 = 70.0;     // lower leg

float r2d(float r) { return r * 57.2958; }

// ===== INVERSE KINEMATICS FOR LEFT FRONT LEG =====
void moveToXY(float x, float y)
{
float d = sqrt(x*x + y*y);

float cosK = (d*d - L1*L1 - L2*L2) / (2 * L1 * L2);
cosK = constrain(cosK, -1.0, 1.0);
float thetaK = acos(cosK);

float phi = atan2(y, x);
float psi = atan2(L2 * sin(thetaK), L1 + L2 * cos(thetaK));
float thetaH = phi - psi;

int hipDeg  = hipZero  + hipDir  * r2d(thetaH);
int kneeDeg = kneeZero + kneeDir * r2d(thetaK);

hipDeg  = constrain(hipDeg,  0, 180);
kneeDeg = constrain(kneeDeg, 0, 180);

hip.write(hipDeg);
knee.write(kneeDeg);
}

void setup() {
hip.attach(hipPin);
knee.attach(kneePin);

// -------- STEP 1: ALIGN BOTH SERVOS TO EXACT 90° --------
for(int pos=0; pos<=90; pos+=2){
hip.write(pos);
knee.write(pos);
delay(20);
}

hip.write(90);
knee.write(90);
delay(1500);   // stop here & check orientation

// -------- STEP 2: MOVE TO IK NEUTRAL POSITION --------
moveToXY(60, -80);   // standing pose
delay(1500);
}

void loop() {
// Empty movement loop for alignment mode
}
