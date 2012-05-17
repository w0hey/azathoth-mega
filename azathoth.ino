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
  link.setHandler(0x03, cmd_lcd);
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

void cmd_lcd(byte length, byte* data) {
  switch (data[0]) {
    case 0x00:
      lcd.clear();
      break;
    
    case 0x01:
      if (data[1] == 0x00) {
        lcd.displayOff();
      }
      else if (data[1] == 0x01) {
        lcd.displayOn();
      }
      break;
    
    case 0x02:
      // TODO
      break;
    
    case 0x03:
      lcd.setPos(data[1], data[2]);
      break;
    
    case 0x04:
      {
        int len = length - 1;
        char *buf = (char*) malloc((len + 1) * sizeof(char));
        if (buf == NULL) {
          // allocation failed
          handleError(E_MALLOC);
        }
        memcpy(buf, data + 2, len);
        buf[len] = 0x00; // terminate the string
        lcd.write(buf);
        free(buf);
        break;
      }
    
    default:
      break;
  }
}

void cmd_sonar(int length, byte* packet) {
}

void cmd_compass(int length, byte* packet) {
}

void cmd_speakjet(int length, byte* packet) {
}

void cmd_estop() {
}

void handleError(byte errcode) {
}
