#pragma once
#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecomm/interrupts.h>
#include <hardwarecomm/port.h>

namespace os {
namespace hardwarecomm {

class PeripheralComponentInterconnectDeviceDescriptor {
public:
  os::common::uint32_t portBase;
  os::common::uint32_t interrupt;

  os::common::uint16_t bus;
  os::common::uint16_t device;
  os::common::uint16_t function;

  os::common::uint16_t vendor_id;
  os::common::uint16_t device_id;

  os::common::uint8_t class_id;
  os::common::uint8_t subclass_id;
  os::common::uint8_t interface_id;

  os::common::uint8_t revision;

  PeripheralComponentInterconnectDeviceDescriptor(){};
  ~PeripheralComponentInterconnectDeviceDescriptor(){};
};

class PeripheralComponentInterconnectController {
private:
  Port32Bit dataPort;
  Port32Bit commandPort;

public:
  PeripheralComponentInterconnectController();
  ~PeripheralComponentInterconnectController();

  os::common::uint32_t Read(os::common::uint16_t bus,
                            os::common::uint16_t device,
                            os::common::uint16_t function,
                            os::common::uint32_t registerOffset);

  void Write(os::common::uint16_t bus, os::common::uint16_t device,
             os::common::uint16_t function, os::common::uint32_t registerOffset,
             os::common::uint32_t value);
  bool DeviceHasFunctions(os::common::uint16_t bus,
                          os::common::uint16_t device);

  void SelectDrivers(os::drivers::DriverManager *drvManager);
  PeripheralComponentInterconnectDeviceDescriptor
  GetDeviceDescriptor(os::common::uint16_t bus, os::common::uint16_t device,
                      os::common::uint16_t function);
};
} // namespace hardwarecomm
} // namespace os