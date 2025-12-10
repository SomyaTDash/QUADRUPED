#include <Servo.h>

Servo hip;
Servo knee;

int hipPin = 9;
int kneePin = 10;

void moveSlow(Servo &s, int target) {
  for (int i = 0; i <= 90; i++) {
    s.write(i);
    delay(5);
  }
  s.write(target);
}

void setup() {
  hip.attach(hipPin);
  knee.attach(kneePin);

  // strong activation pulses
  for (int i = 0; i < 30; i++) {
    hip.write(90);
    knee.write(90);
    delay(20);
  }
}

void loop() {
  // hold position
  hip.write(90);
  knee.write(90);
}
