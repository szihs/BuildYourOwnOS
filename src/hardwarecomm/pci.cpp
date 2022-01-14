#include <drivers/amd_am79c973.h>
#include <hardwarecomm/pci.h>
using namespace os::common;
using namespace os::hardwarecomm;
using namespace os::drivers;

void printf(const char *str);
void printfHex(uint8_t key);

PeripheralComponentInterconnectDeviceDescriptor
PeripheralComponentInterconnectController::GetDeviceDescriptor(
    uint16_t bus, uint16_t device, uint16_t function) {

  PeripheralComponentInterconnectDeviceDescriptor result;
  result.bus = bus;
  result.device = device;
  result.function = function;

  result.vendor_id = Read(bus, device, function, 0x00);
  result.device_id = Read(bus, device, function, 0x02);

  result.class_id = Read(bus, device, function, 0x0b);
  result.subclass_id = Read(bus, device, function, 0x0a);
  result.interface_id = Read(bus, device, function, 0x09);

  result.revision = Read(bus, device, function, 0x08);
  result.interrupt = Read(bus, device, function, 0x3C);
  return result;
}

PeripheralComponentInterconnectController::
    PeripheralComponentInterconnectController()
    : dataPort(0xCFC), commandPort(0xCF8) {}

PeripheralComponentInterconnectController::
    ~PeripheralComponentInterconnectController() {}

uint32_t PeripheralComponentInterconnectController::Read(
    uint16_t bus, uint16_t device, uint16_t function, uint32_t registerOffset) {
  uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) |
                ((function & 0x07) << 8) | (registerOffset & 0xFC);

  commandPort.Write(id);
  uint32_t result = dataPort.Read();

  return result >> (8 * (registerOffset % 4));
}

void PeripheralComponentInterconnectController::Write(uint16_t bus,
                                                      uint16_t device,
                                                      uint16_t function,
                                                      uint32_t registerOffset,
                                                      uint32_t value) {
  uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) |
                ((function & 0x07) << 8) | (registerOffset & 0xFC);
  commandPort.Write(id);
  dataPort.Write(value);
}
bool PeripheralComponentInterconnectController::DeviceHasFunctions(
    uint16_t bus, uint16_t device) {

  return Read(bus, device, 0, 0x0E) & (1 << 7);
}

Driver *PeripheralComponentInterconnectController::GetDriver(
    PeripheralComponentInterconnectDeviceDescriptor dev,
    os::hardwarecomm::InterruptManager *interrupts) {

  Driver *driver = 0;
  switch (dev.vendor_id) {
  case 0x1022: // AMD am79c973
    switch (dev.device_id) {
    case 0x2000:
      driver = (amd_am79c973 *)MemoryManager::activeMemoryManager->malloc(
          sizeof(amd_am79c973));
      if (driver)
        new (driver) amd_am79c973(&dev, interrupts);
      printf("AMD am79c973 ");
      return driver;
      break;
    }
    break;

  case 0x8086: // Intel
               //  printf("Intel Device\n");
    break;
  }

  switch (dev.class_id) {
  case 0x03: // graphics
    switch (dev.subclass_id) {
    case 0x00: // VGA
      printf("VGA ");
      break;
    }
  }
  return 0;
}

BaseAddressRegister
PeripheralComponentInterconnectController::GetBaseAddressRegister(
    uint16_t bus, uint16_t device, uint16_t function, uint16_t bar) {

  BaseAddressRegister result;

  uint32_t headerType = Read(bus, device, function, 0x0E) & 0x7F;
#if 0
  printf("HeaderType is 0x");
  printfHex((headerType >> 24) & 0xFF);
  printfHex((headerType >> 16) & 0xFF);
  printfHex((headerType >> 8) & 0xFF);
  printfHex(headerType & 0xFF);
  printf("\n");
#endif
  uint16_t maxBars = 6 - (4 * headerType);
  if (bar >= maxBars) {
    return result;
  }

  uint32_t bar_value = Read(bus, device, function, 0x10 + 4 * bar);
  result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;

  __attribute__((unused)) uint32_t temp; // to check rw bits

  if (result.type == MemoryMapping) {
    switch ((bar_value >> 1) & 0x3) {
    case 0: // 32 bit Mode
    case 1: // 20 bit mode
    case 2: // 64 bit mode
      break;
    }

    result.prefetchable = (((bar_value >> 3) & 0x1) == 0x1);

  } else {
    result.address = (uint8_t *)(bar_value & ~0x3);
    result.prefetchable = false;
  }
  return result;
}

void PeripheralComponentInterconnectController::SelectDrivers(
    DriverManager *drvManager, InterruptManager *interrupts) {
  for (uint16_t bus = 0; bus < 8; bus++) {
    for (uint16_t device = 0; device < 32; device++) {
      uint16_t numFuncs = DeviceHasFunctions(bus, device) ? 8 : 1;
      for (uint16_t func = 0; func < numFuncs; func++) {
        PeripheralComponentInterconnectDeviceDescriptor dev =
            GetDeviceDescriptor(bus, device, func);

        if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
          continue;

        for (uint8_t barNum = 0; barNum < 6; barNum++) {
          BaseAddressRegister bar =
              GetBaseAddressRegister(bus, device, func, barNum);
          if (bar.address && (bar.type == InputOutput)) {
            dev.portBase = (uint32_t)bar.address;
          }
        }

        Driver *driver = GetDriver(dev, interrupts);
        if (driver) {
          drvManager->AddDriver(driver);
        }

        printf("PCI BUS ");
        printfHex(bus & 0xFF);

        printf(", DEVICE ");
        printfHex(device & 0xFF);

        printf(", FUNCTION ");
        printfHex(func & 0xFF);

        printf(" = VENDOR ");
        printfHex((dev.vendor_id & 0xFF00) >> 8);
        printfHex(dev.vendor_id & 0xFF);
        printf(", DEVICE ");
        printfHex((dev.device_id & 0xFF00) >> 8);
        printfHex(dev.device_id & 0xFF);
        printf("\n");
      }
    }
  }
}
