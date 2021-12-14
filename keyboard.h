#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "interrupts.h"
#include "port.h"
#include "types.h"

class KeyboardDriver : public InterruptHandler {
private:
  Port8Bit dataport;
  Port8Bit commandport;
  virtual uint32_t doHandleInterrupt(uint32_t esp);

public:
  KeyboardDriver(InterruptManager *manager);
  ~KeyboardDriver();
};

#endif