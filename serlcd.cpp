// Interface to the sparkfun serial LCD

#include "serlcd.h"

SerLCD::SerLCD() {
  // TODO: Make serial port selectable
  Serial1.begin(9600);
}

// Clears the LCD display
void SerLCD::clear() {
  Serial1.write(0xfe); // Command char
  Serial1.write(0x01); // Clear display
}

// Prints the provided characters to the LCD
// Throws them directly to the LCD, no processing is done.
void SerLCD::write(char* text) {
  Serial1.write(text);
}
  
