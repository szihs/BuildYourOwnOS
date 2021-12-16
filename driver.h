#pragma once
#include "types.h"
class Driver {
private:
  virtual void doActivate();
  virtual void doDeactivate();

  virtual int32_t doReset();

public:
  Driver();
  ~Driver();
  void Activate();
  void Deactivate();

  int32_t Reset();
};

class DriverManager {

private:
  Driver *drivers[256];
  int numDrivers;

public:
  DriverManager();
  ~DriverManager();
  void AddDriver(Driver *);
  void ActivateAll();
};