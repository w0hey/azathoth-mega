#include "compass.h"
#include <Arduino.h>
#include <Wire.h>

Compass::Compass() {
  Wire.begin();
  // we only use the 7 most significant bits of the address
  compassAddr = COMPASS_ADDRESS >> 1;
}

// Query the compass and return the heading in degrees. Incurs a 6ms delay
float Compass::getHeading() {
  byte val = 0;
  byte data[2];
  int pos, frac;
  
  Wire.beginTransmission(compassAddr);
  // Request heading read
  Wire.write(0x41); // "A"
  Wire.endTransmission();
  // TODO Azathoth hates delays, and I should find another way to do this
  delay(8); // Datasheet asks for a minumum 6 ms delay
  
  // Request two bytes from compass address
  Wire.requestFrom(compassAddr, 2);
  pos = 0;
  while(Wire.available()) {
    char c = Wire.read();
    data[pos] = c;
    pos++;
  }
  // FIXME: It feels like this should be done with a shift operation:
  frac = data[0]*256 + data[1];
  
  return (frac/10.0);
}

// Bring the compass out of sleep mode
void Compass::wake() {
  Wire.beginTransmission(compassAddr);
  Wire.write(0x57); // "W"
  Wire.endTransmission();
}

// Put the compass into sleep mode
void Compass::sleep() {
  Wire.beginTransmission(compassAddr);
  Wire.write(0x53); // "S"
  Wire.endTransmission();
}

// Enter user calibration mode
void Compass::enterCalibration() {
  Wire.beginTransmission(compassAddr);
  Wire.write(0x43); // "C"
  Wire.endTransmission();
}

// Exit user calibration mode
void Compass::exitCalibration() {
  Wire.beginTransmission(compassAddr);
  Wire.write(0x45); // "E"
  Wire.endTransmission();
}

  
