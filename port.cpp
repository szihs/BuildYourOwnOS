#include "port.h"

Port::Port(uint16_t portnumber) { this->portNumber = portnumber; }

Port::~Port() {}

Port8Bit::Port8Bit(uint16_t portnumber) : Port(portnumber) {}

Port8Bit::~Port8Bit(){};

void Port8Bit::doWrite(uint8_t data) {
  __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(portNumber));
}
uint8_t Port8Bit::doRead() {
  uint8_t result;
  __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(portNumber));
  return result;
}
void Port8Bit::Write(uint8_t data) { return doWrite(data); }
uint8_t Port8Bit::Read() { return doRead(); }

Port8BitSlow::Port8BitSlow(uint16_t portnumber) : Port8Bit(portnumber) {}

Port8BitSlow::~Port8BitSlow(){};

void Port8BitSlow::doWrite(uint8_t data) {
  __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:"
                   :
                   : "a"(data), "Nd"(portNumber));
}

Port16Bit::Port16Bit(uint16_t portnumber) : Port(portnumber) {}

Port16Bit::~Port16Bit(){};

void Port16Bit::doWrite(uint16_t data) {
  __asm__ volatile("outw %0, %1" : : "a"(data), "Nd"(portNumber));
}
uint16_t Port16Bit::doRead() {
  uint16_t result;
  __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(portNumber));
  return result;
}

void Port16Bit::Write(uint16_t data) { return doWrite(data); }
uint16_t Port16Bit::Read() { return doRead(); }

Port32Bit::Port32Bit(uint16_t portnumber) : Port(portnumber) {}

Port32Bit::~Port32Bit(){};

void Port32Bit::doWrite(uint32_t data) {
  __asm__ volatile("outl %0, %1" : : "a"(data), "Nd"(portNumber));
}
uint32_t Port32Bit::doRead() {
  uint32_t result;
  __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(portNumber));
  return result;
}

void Port32Bit::Write(uint32_t data) { return doWrite(data); }
uint32_t Port32Bit::Read() { return doRead(); }