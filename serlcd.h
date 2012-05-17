// Header for interfacing the Sparkfun serial LCD

#ifndef _SERLCD_H_
#define _SERLCD_H_

#include <Arduino.h>

#define BAUD_RESET 0x12 // <Control>-R

#define E_CMD 0xfe
#define ECMD_CLEAR 0x01
#define ECMD_DISPLAY_OFF 0x08
#define ECMD_DISPLAY_ON 0x0c
#define ECMD_POS 0x80

class SerLCD {
  public:
    SerLCD();
    void clear();
    void write(char*);
    void printDec(int);
    void backlight();
    void setPos(byte, byte);
    void displayOn();
    void displayOff();
    void handle(byte, byte*);
  private:
};

#endif
