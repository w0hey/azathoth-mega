// Header file for BB->Mega datalink

#ifndef _LINK_H_
#define _LINK_H_
#include <Arduino.h>

typedef struct {
  byte command;
  void (*handler)(byte, byte*);
} handler_t;

class Link {
  public:
    Link(void(*)(byte));
    void setHandler(byte, void(*)(byte, byte*));
    void service();
    void sendData(int, byte*);
  private:
    byte packet[255];
    byte packet_out[255];
    byte recv;
    int pos;
    byte len;
    boolean xor_next;
    int nHandlers;
    handler_t* handlers;
    void buildPacket(byte, byte*);
    void (*errHandler)(byte);
    void dispatch(byte, byte*);
    void (*getHandler(byte))(byte, byte*);
    static int compare_key(const void*, const void*);
    static int compare(const void*, const void*);
};

#endif
