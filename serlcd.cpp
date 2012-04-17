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


// Set the current cursor position to the provided line and column
void SerLCD::set_pos(byte line, byte column) {
  byte pos = 0;
  if (line == 0) {
    pos = column;
  }
  if (line == 1) {
    pos = column + 64;
  }
  Serial1.write(0xfe);
  Serial1.write(0x80 + pos);
}

// Blanks the display
void SerLCD::display_on() {
  Serial1.write(0xfe);
  Serial1.write(0x0c);
}

// Unblanks the display
void SerLCD::display_off() {
  Serial1.write(0xfe);
  Serial1.write(0x08);
}
