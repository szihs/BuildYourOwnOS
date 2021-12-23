#include <drivers/driver.h>
using namespace os::common;
using namespace os::drivers;

void printf(const char *);

Driver::Driver() {}

Driver::~Driver() {}

void Driver::doActivate() { printf("Base doActivate\n"); }
void Driver::doDeactivate() {}
int32_t Driver::doReset() {}

void Driver::Activate() { doActivate(); }
void Driver::Deactivate() { doDeactivate(); }

int32_t Driver::Reset() { return doReset(); }

DriverManager::DriverManager() { numDrivers = 0; }
DriverManager::~DriverManager() {}
void DriverManager::AddDriver(Driver *drv) {
  drivers[numDrivers] = drv;
  numDrivers++;
}

void DriverManager::ActivateAll() {
  for (uint32_t i = 0; i < numDrivers; i++) {
    drivers[i]->Activate();
  }
}