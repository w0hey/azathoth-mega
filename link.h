// Header file for BB->Mega datalink

#ifndef _LINK_H_
#define _LINK_H_
#include <Arduino.h>

class Link {
  public:
    Link(void (*function)(int, byte*));
    void service();
    void send_data(int, byte*);
  private:
    byte packet[255];
    byte packet_out[255];
    byte recv;
    int pos;
    int len;
    boolean xor_next;
    boolean in_packet;
    void (*callback)(int, byte*);
    
    void get_length();
    void build_packet(int, byte*);
};

#endif