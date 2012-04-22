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

// Misc variables
boolean led_on = false; // Keeps track of the current onboard LED state

// Interfaces
Link link = Link(dispatch_packet);
SerLCD lcd = SerLCD();
Sonar sonar = Sonar();

// Psuedothreads
// Update sonar every 100ms by default
TimedAction sonarAction = TimedAction(100, update_sonar);

void setup() {
  lcd.clear();
  lcd.write("Initializing...");
  init_io();
  delay(500);
  // leave the sonar disabled until we want it.
  sonarAction.disable();
}

void loop() {

}

void serialEvent() {
  link.service();
}

void dispatch_packet(int length, byte* packet) {
  return;
}

void init_io() {
  // Without knowing *why* we're booting/rebooting, let's just make sure
  // the unstoppable 300-pound metal monster is NOT moving:
  pinMode(P_ESTOP, OUTPUT);
  digitalWrite(P_ESTOP, LOW);
  
  pinMode(P_LED, OUTPUT);
  pinMode(P_SONAR_EN, OUTPUT);
  pinMode(P_SONAR_PW, INPUT);
  digitalWrite(P_LED, HIGH);
  // Command a ranging cycle just to let the sensor calibrate
  lcd.setPos(1,0);
  lcd.write("Cal SONAR..");
  sonar.ping();
  lcd.write("done");
}

void update_sonar() {
  // Stub, since we can't just pass sonar.update() to TimedAction (grumble)
  sonar.update();
  return;
}

