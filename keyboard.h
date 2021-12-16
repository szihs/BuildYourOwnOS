#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "driver.h"
#include "interrupts.h"
#include "port.h"
#include "types.h"

class KeyboardDriver : public InterruptHandler, public Driver {
private:
  Port8Bit dataport;
  Port8Bit commandport;
  virtual uint32_t doHandleInterrupt(uint32_t esp);
  virtual void doActivate();

public:
  KeyboardDriver(InterruptManager *manager);
  ~KeyboardDriver();
};

#endif