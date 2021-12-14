#ifndef __PORT_H
#define __PORT_H
#include "types.h"

class Port {
protected:
  uint16_t portNumber;
  Port(uint16_t portNumber); // purely virtual base class
  ~Port();
};

class Port8Bit : public Port {
public:
  Port8Bit(uint16_t portNumber);
  ~Port8Bit();
  void Write(uint8_t data);
  uint8_t Read();

private:
  virtual void doWrite(uint8_t data);
  virtual uint8_t doRead();
};

class Port8BitSlow : public Port8Bit {
public:
  Port8BitSlow(uint16_t portNumber);
  ~Port8BitSlow();

private:
  virtual void doWrite(uint8_t data);
};
class Port16Bit : public Port {
public:
  Port16Bit(uint16_t portNumber);
  ~Port16Bit();
  void Write(uint16_t data);
  uint16_t Read();

private:
  virtual void doWrite(uint16_t data);
  virtual uint16_t doRead();
};
class Port32Bit : public Port {
public:
  Port32Bit(uint16_t portNumber);
  ~Port32Bit();
  void Write(uint32_t data);
  uint32_t Read();

private:
  virtual void doWrite(uint32_t data);
  virtual uint32_t doRead();
};
#endif