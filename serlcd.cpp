// Interface to the sparkfun serial LCD

#include "serlcd.h"

SerLCD::SerLCD() {
  // TODO: Make serial port selectable
  Serial1.begin(9600);
}

void SerLCD::clear() {
  Serial1.write(0xfe); // Command char
  Serial1.write(0x01); // Clear display
}

void SerLCD::write(char* text) {
  Serial1.write(text);
}
  
