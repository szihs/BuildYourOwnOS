#pragma once
#include <common/types.h>
#include <hardwarecomm/interrupts.h>
#include <multitasking.h>

namespace os {
class SyscallHandler : public hardwarecomm::InterruptHandler {
private:
  virtual common::uint32_t doHandleInterrupt(common::uint32_t esp);

public:
  SyscallHandler(common::uint8_t interruptNumber,
                 hardwarecomm::InterruptManager *InterruptManager);
  ~SyscallHandler();
};
} // namespace os