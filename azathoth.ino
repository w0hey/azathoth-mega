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
TimedAction sonarSender = TimedAction(500, send_sonar);
TimedAction compassAction = TimedAction(100, update_compass);
TimedAction adcAction = TimedAction(2000, update_adc);

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  pinMode(P_LED, OUTPUT);
  delay(1000); // wait for serial lcd to settle before touching it.
  lcd.clear();
  lcd.write("Initializing...");
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
  sonarSender.check();
  adcAction.check();
}

void serialEvent() {
  link.service();
}

void update_sonar() {
  // Stub, since we can't just pass sonar.update() to TimedAction (grumble)
  sonar.update();
  byte range = sonar.getRange();
  if (range < 24) {
    byte data[2] = {0x40, range};
    link.sendData(2, data);
  };
  return;
}

void send_sonar() {
  byte range = sonar.getRange();
  byte data[2] = {0x40, range};
  link.sendData(2, data);
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
