// Header for interfacing the Sparkfun serial LCD

#ifndef _SERLCD_H_
#define _SERLCD_H_

#include <Arduino.h>

class SerLCD {
  public:
    SerLCD();
    void clear();
    void write(char*);
    void print_dec(int);
    void backlight();
    void set_pos(byte, byte);
    void display_on();
    void display_off();
  private:
};

#endif
