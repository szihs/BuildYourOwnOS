#pragma once
#include "interrupts.h"
#include "port.h"
#include "types.h"

class MouseDriver : public InterruptHandler {
private:
  Port8Bit dataport;
  Port8Bit commandport;

  uint8_t buffer[3];
  uint8_t offset;
  uint8_t buttons;


public:
  MouseDriver(InterruptManager *manager);
  ~MouseDriver();
  virtual uint32_t HandleInterrupt(uint32_t esp);
};

