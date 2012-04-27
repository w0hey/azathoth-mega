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

// Interfaces
Link link = Link(dispatch_packet);
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
  delay(500);
  // leave the sonar disabled until we want it.
  sonarAction.disable();
}

void loop() {
  return;
}

void serialEvent() {
  link.service();
}

// Callback from link, when it has a complete packet to process
void dispatch_packet(int length, byte* packet) {
  switch (packet[2]) {
    case 0x01:
      cmd_connect();
      break;
    case 0x02:
      cmd_disconnect();
      break;
    case 0x03:
      cmd_lcd(length, packet);
      break;
    case 0x04:
      cmd_sonar(length, packet);
      break;
    case 0x05:
      cmd_compass(length, packet);
      break;
    case 0x06:
      cmd_speakjet(length, packet);
      break;
    case 0xff:
      cmd_estop();
      break;
    default:
      break;
  }
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

void cmd_connect() {
  lcd.clear();
  lcd.write("Connected");
}

void cmd_disconnect() {
  lcd.clear();
  lcd.write("Disconnected");
}

void cmd_lcd(int length, byte* packet) {
  switch (packet[3]) {
    case 0x00:
      lcd.clear();
      break;
    
    case 0x01:
      if (packet[4] == 0x00) {
        lcd.displayOff();
      }
      else if (packet[4] == 0x01) {
        lcd.displayOn();
      }
      break;
    
    case 0x02:
      // TODO
      break;
    
    case 0x03:
      lcd.setPos(packet[4], packet[5]);
      break;
    
    case 0x04:
      {
        int len = length - 4;
        char *buf = (char*) malloc((len + 1) * sizeof(char));
        if (buf == NULL) {
          // allocation failed
          break;
        }
        memcpy(buf, packet + 4, len);
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
