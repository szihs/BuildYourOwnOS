#pragma once
#include <common/types.h>
namespace os {
namespace drivers {
class Driver {
private:
  virtual void doActivate();
  virtual void doDeactivate();

  virtual os::common::int32_t doReset();

public:
  Driver();
  ~Driver();
  void Activate();
  void Deactivate();

  os::common::int32_t Reset();
};

class DriverManager {

private:
  Driver *drivers[256];
  os::common::uint32_t numDrivers;

public:
  DriverManager();
  ~DriverManager();
  void AddDriver(Driver *);
  void ActivateAll();

  Driver* getDriver(common::uint8_t index) const { return drivers[index];}
};
} // namespace drivers
} // namespace os