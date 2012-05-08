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
// Let's make this a bit smarter..

void dispatch_packet(int length, byte* packet) {
  len = packet[1] - 1 // we don't need the first payload byte
  cmd = packet[2] // first payload byte
  // get a buffer of size len
  byte *data = (byte*) malloc((len) * sizeof(byte));
  if (data == null) {
    // malloc fail!
    handleError(E_MALLOC);
    return;
  }
  memcpy(data, packet + 3, len);
  switch (cmd) {
    case 0x01:
      cmd_connect();
      break;
    case 0x02:
      cmd_disconnect();
      break;
    case 0x03:
      cmd_lcd(len, data);
      break;
    case 0x04:
      cmd_sonar(len, data);
      break;
    case 0x05:
      cmd_compass(len, data);
      break;
    case 0x06:
      cmd_speakjet(len, data);
      break;
    case 0xff:
      cmd_estop();
      break;
    default:
      break;
  }
  free(data);
  data = null;
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

void cmd_lcd(int length, byte* data) {
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

void handleError(int errcode) {
}
