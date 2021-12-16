#pragma once
#include "driver.h"
#include "interrupts.h"
#include "port.h"
#include "types.h"

class MouseDriver : public InterruptHandler, public Driver {
private:
  Port8Bit dataport;
  Port8Bit commandport;

  uint8_t buffer[3];
  uint8_t offset;
  uint8_t buttons;

  virtual uint32_t doHandleInterrupt(uint32_t esp);
  virtual void doActivate();

public:
  MouseDriver(InterruptManager *manager);
  ~MouseDriver();
};
