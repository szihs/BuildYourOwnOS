#pragma once
#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecomm/interrupts.h>
#include <hardwarecomm/port.h>

namespace os {
namespace drivers {
class MouseEventHandler {
private:
  virtual void doOnMouseDown(os::common::uint8_t button);
  virtual void doOnMouseUp(os::common::uint8_t button);
  virtual void doOnMouseMove(os::common::int8_t x, os::common::int8_t y);
  virtual void doOnActivate();

public:
  MouseEventHandler() {}
  ~MouseEventHandler() {}
  void OnMouseDown(os::common::uint8_t button);
  void OnMouseUp(os::common::uint8_t button);
  void OnMouseMove(os::common::int8_t x, os::common::int8_t y);
  void OnActivate();
};

class MouseDriver : public os::hardwarecomm::InterruptHandler,
                    public os::drivers::Driver {
private:
  os::hardwarecomm::Port8Bit dataport;
  os::hardwarecomm::Port8Bit commandport;

  os::common::uint8_t buffer[3];
  os::common::uint8_t offset;
  os::common::uint8_t buttons;
  MouseEventHandler *handler;
  virtual os::common::uint32_t doHandleInterrupt(os::common::uint32_t esp);
  virtual void doActivate();

public:
  MouseDriver(os::hardwarecomm::InterruptManager *manager,
              MouseEventHandler *handler);
  ~MouseDriver();
};
} // namespace drivers
} // namespace os
