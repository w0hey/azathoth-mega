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


// Function to be called when there is pending serial data to process
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

// Takes the provided data, wraps it in a packet header, and sends it.
void Link::send_data(int size, byte data[]) {
  build_packet(size, data);
  Serial.write(packet_out[0]); // Send start byte, successive bytes must be escaped if necessary
  // skip start byte, add 3 to size to include header
  byte b;
  for (int i = 1; i <= size + 3; i++) {
    b = packet_out[i];
    if (b == 0x7e || b == 0x7d || b == 0x11 || b == 0x13) {
      // byte must be escaped
      b = b ^ 0x20;
      Serial.write(0x7d); // escape marker
    }
    Serial.write(b);
  }
}

void Link::get_length() {
  // read the packet header and set the len variable accordingly
  len = (packet[1] << 8) + packet[2];
}

// adds a packet header to the provided data for transmission
void Link::build_packet(int size, byte data[]) {
  packet_out[0] = 0x7e; // start byte
  packet_out[1] = highByte(size + 3); // Length including header
  packet_out[2] = lowByte(size + 3);
  for (int i = 0; i <= size; i++) {
    packet_out[3 + i] = data[i];
  }
}
