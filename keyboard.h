#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "interrupts.h"
#include "port.h"
#include "types.h"

class KeyboardDriver : public InterruptHandler {
private:
  Port8Bit dataport;
  Port8Bit commandport;

public:
  KeyboardDriver(InterruptManager *manager);
  ~KeyboardDriver();
  virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif