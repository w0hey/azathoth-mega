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

// Misc variables
boolean led_on = false; // Keeps track of the current onboard LED state

// Interfaces
Link link = Link(dispatch_packet);
Sonar sonar = Sonar();

// Psuedothreads
// Update sonar every 100ms by default
TimedAction sonarAction = TimedAction(100, update_sonar);

void setup() {
  init_io();
  // leave the sonar disabled until we want it.
  sonarAction.disable();
  // TODO: move this into link.cpp
  //wait_for_handshake();
}

void loop() {
}

void serialEvent() {
  link.service();
}

void dispatch_packet(int length, byte* packet) {
  return;
}

void wait_for_handshake() {
  byte recv = 0;
  while (recv != 0xd0) {
    recv = Serial.read();
    // Toggle the onboard LED about once a second, probably
    if (millis() % 1000 == 0) {
      digitalWrite(13, led_on ? HIGH : LOW);
      led_on = !led_on;
    }
  }
  led_on = true;
  digitalWrite(13, HIGH);
}

void init_io() {
  pinMode(P_LED, INPUT);
  digitalWrite(P_LED, HIGH);
  sonar.ping();
}

void update_sonar() {
  // Stub, since we can't just pass sonar.update() to TimedAction (grumble)
  sonar.update();
  return;
}

void comm_error(int errcode) {
  digitalWrite(P_LED, HIGH);
}
