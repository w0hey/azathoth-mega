// Header for interfacing the Sparkfun serial LCD

#ifndef _SERLCD_H_
#define _SERLCD_H_

#include <Arduino.h>

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
  private:
};

#endif
