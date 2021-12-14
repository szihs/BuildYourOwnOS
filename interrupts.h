#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H
#include "gdt.h"
#include "port.h"
#include "types.h"

#define N_ENTRIES 256
class InterruptManager;

class InterruptHandler {
protected:
  uint8_t interruptNumber;
  InterruptManager *interruptManager;

  InterruptHandler(uint8_t interruptNumber, InterruptManager *InterruptManager);
  ~InterruptHandler();

private:
  virtual uint32_t doHandleInterrupt(uint32_t esp);

public:
  uint32_t HandleInterrupt(uint32_t esp);
};

class InterruptManager {
  friend class InterruptHandler;

protected:
  static InterruptManager *ActiveInterruptManager;
  InterruptHandler *handlers[N_ENTRIES];
  struct GateDescriptor {
    uint16_t handlerAddressLowBits;
    uint16_t gdt_codeSegmentSelector;
    uint8_t reserved;
    uint8_t access;
    uint16_t handledrAddressHighBits;

  } __attribute__((packed));

  static GateDescriptor interruptDescriptorTable[N_ENTRIES];

  struct InterruptDescriptorTablePointer {
    uint16_t size;
    uint32_t base;
  } __attribute__((packed));

  static void SetInterruptDescriptorTableEntry(
      uint8_t interruptNumber, uint16_t codeSegmentSelectorOffset,
      void (*handler)(), uint8_t DescriptorPrivelegeLevel,
      uint8_t DescriptorType);

  Port8BitSlow picMasterCommand;
  Port8BitSlow picMasterData;
  Port8BitSlow picSlaveCommand;
  Port8BitSlow picSlaveData;

public:
  InterruptManager(GlobalDescriptorTable *gdt);
  ~InterruptManager();
  void Activate();
  void Deactivate();
  static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
  uint32_t DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp);

  static void IgnoreInterruptRequest();

  static void HandleInterruptRequest0x00();
  static void HandleInterruptRequest0x01();
  static void HandleInterruptRequest0x0C();
};

#endif
