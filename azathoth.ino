/* Initial draft of code for Arduino Mega on the new robot.
This code is designed to receive commands from, and return responses to
the Beagleboard.
*/
#include <TimedAction.h>
#include <Wire.h>

#include "pinout.h"
#include "errors.h"
#include "link.h"
#include "sonar.h"
#include "compass.h"
#include "serlcd.h"

int vBat = 0;
int v12 = 0;
int v5 = 0;

// Interfaces
Link link = Link(handleError);
SerLCD lcd = SerLCD(handleError);
Sonar sonar = Sonar();
Compass compass = Compass();

// Psuedothreads
// Update sonar every 100ms by default
TimedAction sonarAction = TimedAction(100, update_sonar);
TimedAction compassAction = TimedAction(100, update_compass);
TimedAction adcAction = TimedAction(2000, update_adc);

void setup() {
  Serial.begin(115200);
  lcd.clear();
  lcd.write("Initializing...");
  init_io();
  // leave the sonar disabled until we want it.
  sonarAction.disable();
  compassAction.disable();
  link.setHandler(0x03, lcd_handler);
  link.setHandler(0x04, sonar_handler);
  byte data[1] = {0x01};
  link.sendData(1, data); // Let the BB know we're alive
}

void loop() {
  sonarAction.check();
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

void update_compass() {
}

void update_adc() {
  vBat = analogRead(ADC_24V);
  v12 = analogRead(ADC_12V);
  v5 = analogRead(ADC_5V);
}

void lcd_handler(byte length, byte* data) {
  lcd.handle(length, data);
}

void sonar_handler(byte length, byte* data) {
  sonar.handle(length, data);
}

void handleError(byte errcode) {
}
