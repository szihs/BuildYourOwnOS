#pragma once

#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecomm/interrupts.h>
#include <hardwarecomm/pci.h>
#include <hardwarecomm/port.h>

namespace os {
namespace drivers {

class amd_am79c973 : public Driver, public hardwarecomm::InterruptHandler {

  struct InitializationBlock {
    common::uint16_t mode;
    unsigned reserved1 : 4;
    unsigned numSendBuffers : 4;
    unsigned reserved2 : 4;
    unsigned numRecvBuffers : 4;
    common::uint64_t physicalAddress : 48;
    common::uint16_t reserved3;
    common::uint64_t logicalAddress;
    common::uint32_t recvBufferDescAddress;
    common::uint32_t sendBufferDescAddress;
  } __attribute__((packed));

  struct BufferDescriptor {
    common::uint32_t address;
    common::uint32_t flags;
    common::uint32_t flags2;
    common::uint32_t avail;
  } __attribute__((packed));

private:
  hardwarecomm::Port16Bit MACAddress0Port;
  hardwarecomm::Port16Bit MACAddress2Port;
  hardwarecomm::Port16Bit MACAddress4Port;
  hardwarecomm::Port16Bit registerDataPort;
  hardwarecomm::Port16Bit registerAddressPort;
  hardwarecomm::Port16Bit resetPort;
  hardwarecomm::Port16Bit busControlRegisterDataPort;

  InitializationBlock initBlock;
  BufferDescriptor *sendBufferDescr;
  BufferDescriptor *recvBufferDescr;

  common::uint8_t sendBufferDescMemory[2048 + 15];
  common::uint8_t recvBufferDescMemory[2048 + 15];
  common::uint8_t currentSendBuffer;
  common::uint8_t currentRecvBuffer;

  common::uint8_t sendBuffers[2 * 1024 + 15][8];
  common::uint8_t recvBuffers[2 * 1024 + 15][8];

  virtual void doActivate();
  virtual void doDeactivate();

  virtual os::common::int32_t doReset();
  virtual os::common::uint32_t doHandleInterrupt(os::common::uint32_t esp);

public:
  amd_am79c973(
      hardwarecomm::PeripheralComponentInterconnectDeviceDescriptor *dev,
      hardwarecomm::InterruptManager *interrupts);
  ~amd_am79c973();
};
} // namespace drivers
} // namespace os