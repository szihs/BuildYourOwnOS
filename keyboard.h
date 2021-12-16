#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "driver.h"
#include "interrupts.h"
#include "port.h"
#include "types.h"

class KeyboardEventHandler {
private:
  virtual void doOnKeyDown(char);
  virtual void doOnKeyUp(char);

public:
  KeyboardEventHandler();
  ~KeyboardEventHandler();
  void OnKeyDown(char);
  void OnKeyUp(char);
};

class KeyboardDriver : public InterruptHandler, public Driver {
private:
  Port8Bit dataport;
  Port8Bit commandport;
  virtual uint32_t doHandleInterrupt(uint32_t esp);
  virtual void doActivate();
  KeyboardEventHandler *handler;

public:
  KeyboardDriver(InterruptManager *manager, KeyboardEventHandler *handler);
  ~KeyboardDriver();
};

#endif