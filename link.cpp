// bleh

#include "link.h"
#include <Arduino.h>

Link::Link(void (*function)(int, byte*)) {
  pos = 0;
  len = 0;
  xor_next = false;
  in_packet = false;
  callback = function;
  Serial.begin(115200);
}

void Link::service() {
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
    
    // check to see if we have the whole header - something about this
    // smells like a hack
    if (pos == 3) {
      get_length();
    }
    
    // check if we're done with this packet
    if (pos == len - 1) {
      callback(len, packet);
      in_packet = false;
      continue;
    }
    
    // on to the next byte
    pos++;
  }
}

void Link::get_length() {
  // read the packet header and set the len variable accordingly
  len = (packet[1] << 8) + packet[2];
}

void Link::build_packet(int size, byte data[]) {
  packet_out[0] = 0x7e; // start byte
  packet_out[1] = highByte(size + 3); // Length including header
  packet_out[2] = lowByte(size + 3);
  for (int i = 0; i <= size; i++) {
    packet_out[3 + i] = data[i];
  }
}
