#pragma once
#include "driver.h"
#include "interrupts.h"
#include "port.h"
#include "types.h"
class MouseEventHandler {
private:
  virtual void doOnMouseDown(uint8_t button);
  virtual void doOnMouseUp(uint8_t button);
  virtual void doOnMouseMove(int8_t x, int8_t y);
  virtual void doOnActivate();

public:
  MouseEventHandler(){}
  ~MouseEventHandler(){}
  void OnMouseDown(uint8_t button);
  void OnMouseUp(uint8_t button);
  void OnMouseMove(int8_t x, int8_t y);
  void OnActivate();
};

class MouseDriver : public InterruptHandler, public Driver {
private:
  Port8Bit dataport;
  Port8Bit commandport;

  uint8_t buffer[3];
  uint8_t offset;
  uint8_t buttons;
  MouseEventHandler *handler;
  virtual uint32_t doHandleInterrupt(uint32_t esp);
  virtual void doActivate();

public:
  MouseDriver(InterruptManager *manager, MouseEventHandler *handler);
  ~MouseDriver();
};
