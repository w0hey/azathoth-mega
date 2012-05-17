/* Initial draft of code for Arduino Mega on the new robot.
This code is designed to receive commands from, and return responses to
the Beagleboard.
*/
#include <TimedAction.h>
#include <Wire.h>

#include "pinout.h"
#include "link.h"
#include "sonar.h"
#include "compass.h"
#include "serlcd.h"

#define E_MALLOC 0

// Interfaces
Link link = Link(handleError);
SerLCD lcd = SerLCD();
Sonar sonar = Sonar();

// Psuedothreads
// Update sonar every 100ms by default
TimedAction sonarAction = TimedAction(100, update_sonar);

void setup() {
  Serial.begin(115200);
  lcd.clear();
  lcd.write("Initializing...");
  init_io();
  // leave the sonar disabled until we want it.
  sonarAction.disable();
  link.setHandler(0x03, lcd_handler);
  byte data[1] = {0x01};
  link.sendData(1, data); // Let the BB know we're alive
}

void loop() {
  return;
}

void serialEvent() {
  link.service();
}

void init_io() {
  pinMode(P_LED, OUTPUT);
  pinMode(P_SONAR_EN, OUTPUT);
  pinMode(P_SONAR_PW, INPUT);
  digitalWrite(P_LED, HIGH);
}

void update_sonar() {
  // Stub, since we can't just pass sonar.update() to TimedAction (grumble)
  sonar.update();
  return;
}

void lcd_handler(byte length, byte* data) {
  lcd.handle(length, data);
}

void handleError(byte errcode) {
}
