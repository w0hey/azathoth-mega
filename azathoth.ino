/* Initial draft of code for Arduino Mega on the new robot.
This code is designed to receive commands from, and return responses to
the Beagleboard.
*/
#include <TimedAction.h>
#include <Wire.h>

#include "pinout.h"
#include "sonar.h"
#include "compass.h"

// Serial packet variables
byte packet[255]; // Buffer for received packet
byte packet_out[255]; // Buffer for pending outbound packet
byte recv; // Buffer for one recieved byte
int pos = 0; // Index into packet buffer at current position
int len = 0; // Packet length as read from header
boolean xor_next = false; // Flag to specify that next byte should be unescaped
boolean in_packet = false; // Flag for packet processing state machine

// Misc variables
boolean led_on = false; // Keeps track of the current onboard LED state

// Interfaces
Sonar sonar = Sonar();

// Psuedothreads
// Update sonar every 100ms by default
TimedAction sonarAction = TimedAction(100, update_sonar);

void setup() {
  Serial.begin(115200); // Is this a logical baud rate?
  init_io();
  // leave the sonar disabled until we want it.
  sonarAction.disable();
  wait_for_handshake();
}

void loop() {
}

void serialEvent() {
  while (Serial.available()) {
    // take a byte
    recv = Serial.read();
    
    // Check for start of packet
    if (!in_packet && recv == 0x7e) {
      pos = 0;
      in_packet = true;
    }
    
    // Check for unexpected start of packet
    if (in_packet && recv == 0x7e) {
      // unexpected start byte, reset the state and attempt to capture
      // the new packet
      pos = 0;
      len = 0;
      xor_next = false;
    }
    
    // check for jabber
    if (!in_packet && recv != 0x7e) {
      // what the fuck, robot? If we're not currently handling a packet,
      // receiving anything other than 0x7e is bullshit.
      continue;
    }
    
    // check to see if we need to unescape the next byte
    if (recv == 0x7d) {
      // discard this byte and unescape the next one
      xor_next = true;
      continue;
    }
    
    // check to see if we need to unescape this byte
    if (xor_next) {
      recv = recv ^ 0x20;
      xor_next = false;
    }
    
    // store the byte
    packet[pos] = recv;
    
    // check to see if we have the whole header - something about this smells like a hack
    if (pos == 3) {
      get_length();
    }
    
    // check if we're done with this packet
    if (pos == len - 1) {
      dispatch_packet();
      in_packet = false;
      continue;
    }
    
    // on to the next byte
    pos++;
  }
}

void build_packet(int size, char data[]) {
  packet_out[0] = 0x7e; // start byte
  packet_out[1] = highByte(size + 3); // Length including header
  packet_out[2] = lowByte(size + 3);
  for (int i = 0; i <= size; i++) {
    packet_out[3 + i] = data[i];
  }
  
}

void send_packet() {
}


void dispatch_packet() {
  return;
}

void get_length() {
  // read the packet header and set the len variable accordingly
  len = (packet[1] << 8) + packet[2];
  return;
}

void wait_for_handshake() {
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
