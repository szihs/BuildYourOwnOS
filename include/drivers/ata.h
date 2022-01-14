#pragma once

#include <common/types.h>
#include <hardwarecomm/port.h>

namespace os {
namespace drivers {
class AdvancedTechnologyAttachment {
private:
  hardwarecomm::Port16Bit dataPort;
  hardwarecomm::Port8Bit errorPort;
  hardwarecomm::Port8Bit sectorCountPort;
  hardwarecomm::Port8Bit lbaLowPort;
  hardwarecomm::Port8Bit lbaMidPort;
  hardwarecomm::Port8Bit lbaHiPort;
  hardwarecomm::Port8Bit devicePort;
  hardwarecomm::Port8Bit commandPort; // for control
  hardwarecomm::Port8Bit controlPort; // used for status msg

  bool master;
  common::uint16_t bytesPerSector;

public:
  AdvancedTechnologyAttachment(common::uint16_t portBase, bool master);
  ~AdvancedTechnologyAttachment();

  void Identify();
  void Read28(common::uint32_t sector);
  void Write28(common::uint32_t sector, common::uint8_t *data,
               common::uint32_t count);
  void Flush();
};
} // namespace drivers
} // namespace os