// Interface to the sparkfun serial LCD

#include "serlcd.h"

SerLCD::SerLCD() {
  // TODO: Make serial port selectable
  Serial1.begin(9600);
}

// Clears the LCD display
void SerLCD::clear() {
  Serial1.write(E_CMD); // Command char
  Serial1.write(ECMD_CLEAR); // Clear display
}

// writes the provided characters to the LCD
// this is just a passthrough for Serial.write()
void SerLCD::write(char* text) {
  Serial1.write(text);
}

// Convenience method to print an integer as a decimal number
void SerLCD::printDec(int i) {
  Serial1.print(i, DEC);
}

// Set the current cursor position to the provided line and column
void SerLCD::setPos(byte line, byte column) {
  byte pos = 0;
  if (line == 0) {
    pos = column;
  }
  if (line == 1) {
    pos = column + 64;
  }
  Serial1.write(E_CMD);
  Serial1.write(ECMD_POS + pos);
}

// Blanks the display
void SerLCD::displayOn() {
  Serial1.write(E_CMD);
  Serial1.write(ECMD_DISPLAY_ON);
}

// Unblanks the display
void SerLCD::displayOff() {
  Serial1.write(E_CMD);
  Serial1.write(ECMD_DISPLAY_OFF);
}
