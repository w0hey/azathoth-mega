#include <Arduino.h>
#include "sonar.h"
#include "pinout.h"


// Constructor
Sonar::Sonar() {
  // this should be taken care of at a higher level, but eh.
  digitalWrite(P_SONAR_EN, LOW);
}

void Sonar::enable() {
  // Cause the sonar to start repeated ranging
  digitalWrite(P_SONAR_EN, HIGH);
}

void Sonar::disable() {
  // Inhibit ranging - power savings?
  digitalWrite(P_SONAR_EN, LOW);
}

void Sonar::update() {
  // Get & store range from the sonar
  int pulsewidth = 0;
  pulsewidth = pulseIn(P_SONAR_PW, HIGH, SONAR_TIMEOUT);
  _lastrange = pulsewidth / SONAR_SCALE;
}

void Sonar::ping() {
  // Start a single ranging cycle, useful only for inital calibration
  // Contains a 30uS delay, as required by the sensor datasheet (>20uS)
  enable();
  delayMicroseconds(30);
  disable();
  delay(250); // don't return until the ranging/calibration cycle is finished
}

byte Sonar::getRange() {
  // Return the last stored range
  return _lastrange;
}
