// Interface to the sparkfun serial LCD

#include "serlcd.h"

SerLCD::SerLCD(void (*errhandler)(byte)) {
  // TODO: Make serial port selectable
  errHandler = errhandler;
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

void SerLCD::handle(byte length, byte* data) {
   switch (data[0]) {
    case 0x00: // Clear LCD
      clear();
      break;
    
    case 0x01: // Set display on/off
      if (data[1] == 0x00) {
        displayOff();
      }
      else if (data[1] == 0x01) {
        displayOn();
      }
      break;
    
    case 0x02: // I forget
      // TODO
      break;
    
    case 0x03: // set cursor position
      setPos(data[1], data[2]);
      break;
    
    case 0x04: // write
      {
        int len = length - 1;
        char *buf = (char*) malloc((len + 1) * sizeof(char));
        if (buf == NULL) {
          // allocation failed
          errHandler(E_MALLOC);
        }
        memcpy(buf, data + 1, len);
        buf[len] = 0x00; // terminate the string
        write(buf);
        free(buf);
        break;
      }
    
    default:
      break;
  }
}
