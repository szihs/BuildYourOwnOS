#pragma once
#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecomm/interrupts.h>
#include <hardwarecomm/port.h>
namespace os {
namespace drivers {
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

class KeyboardDriver : public os::hardwarecomm::InterruptHandler, public Driver {
private:
  os::hardwarecomm::Port8Bit dataport;
  os::hardwarecomm::Port8Bit commandport;
  virtual os::common::uint32_t doHandleInterrupt(os::common::uint32_t esp);
  virtual void doActivate();
  KeyboardEventHandler *handler;

public:
  KeyboardDriver(os::hardwarecomm::InterruptManager *manager, KeyboardEventHandler *handler);
  ~KeyboardDriver();
};

} // namespace drivers
} // namespace os