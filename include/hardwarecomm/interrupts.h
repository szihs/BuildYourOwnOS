#pragma once
#include <common/types.h>
#include <gdt.h>
#include <hardwarecomm/port.h>
#include <multitasking.h>

#define N_ENTRIES 256
namespace os {
namespace hardwarecomm {
class InterruptManager;

class InterruptHandler {
protected:
  os::common::uint8_t interruptNumber;
  InterruptManager *interruptManager;

  InterruptHandler(os::common::uint8_t interruptNumber,
                   InterruptManager *InterruptManager);
  ~InterruptHandler();

private:
  virtual os::common::uint32_t doHandleInterrupt(os::common::uint32_t esp);

public:
  os::common::uint32_t HandleInterrupt(os::common::uint32_t esp);
};

class InterruptManager {
  friend class InterruptHandler;

protected:
  static InterruptManager *ActiveInterruptManager;

  InterruptHandler *handlers[N_ENTRIES];
  TaskManager *taskManager;
  struct GateDescriptor {
    os::common::uint16_t handlerAddressLowBits;
    os::common::uint16_t gdt_codeSegmentSelector;
    os::common::uint8_t reserved;
    os::common::uint8_t access;
    os::common::uint16_t handledrAddressHighBits;

  } __attribute__((packed));

  static GateDescriptor interruptDescriptorTable[N_ENTRIES];

  struct InterruptDescriptorTablePointer {
    os::common::uint16_t size;
    os::common::uint32_t base;
  } __attribute__((packed));

  static void SetInterruptDescriptorTableEntry(
      os::common::uint8_t interruptNumber,
      os::common::uint16_t codeSegmentSelectorOffset, void (*handler)(),
      os::common::uint8_t DescriptorPrivelegeLevel,
      os::common::uint8_t DescriptorType);

  Port8BitSlow picMasterCommand;
  Port8BitSlow picMasterData;
  Port8BitSlow picSlaveCommand;
  Port8BitSlow picSlaveData;

public:
  InterruptManager(GlobalDescriptorTable *gdt, TaskManager *taskManager);
  ~InterruptManager();
  void Activate();
  void Deactivate();
  static os::common::uint32_t
  handleInterrupt(os::common::uint8_t interruptNumber,
                  os::common::uint32_t esp);
  os::common::uint32_t DoHandleInterrupt(os::common::uint8_t interruptNumber,
                                         os::common::uint32_t esp);

  static void IgnoreInterruptRequest();

  static void HandleInterruptRequest0x00();
  static void HandleInterruptRequest0x01();
  static void HandleInterruptRequest0x09();
  static void HandleInterruptRequest0x0C();
  static void HandleInterruptRequest0x60();

};

} // namespace hardwarecomm
} // namespace os