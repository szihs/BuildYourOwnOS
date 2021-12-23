#pragma once
#include <common/types.h>
namespace os {
namespace hardwarecomm {
class Port {
protected:
  os::common::uint16_t portNumber;
  Port(os::common::uint16_t portNumber); // purely virtual base class
  ~Port();
};

class Port8Bit : public Port {
public:
  Port8Bit(os::common::uint16_t portNumber);
  ~Port8Bit();
  void Write(os::common::uint8_t data);
  os::common::uint8_t Read();

private:
  virtual void doWrite(os::common::uint8_t data);
  virtual os::common::uint8_t doRead();
};

class Port8BitSlow : public Port8Bit {
public:
  Port8BitSlow(os::common::uint16_t portNumber);
  ~Port8BitSlow();

private:
  virtual void doWrite(os::common::uint8_t data);
};
class Port16Bit : public Port {
public:
  Port16Bit(os::common::uint16_t portNumber);
  ~Port16Bit();
  void Write(os::common::uint16_t data);
  os::common::uint16_t Read();

private:
  virtual void doWrite(os::common::uint16_t data);
  virtual os::common::uint16_t doRead();
};
class Port32Bit : public Port {
public:
  Port32Bit(os::common::uint16_t portNumber);
  ~Port32Bit();
  void Write(os::common::uint32_t data);
  os::common::uint32_t Read();

private:
  virtual void doWrite(os::common::uint32_t data);
  virtual os::common::uint32_t doRead();
};
} // namespace hardwarecomm
} // namespace os