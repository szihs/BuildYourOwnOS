#include "keyboard.h"
void printf(const char *str);

KeyboardDriver::KeyboardDriver(InterruptManager *manager)
    : InterruptHandler(0x21, manager), dataport(0x60), commandport(0x64) {
  while (commandport.Read() & 0x1) {
    dataport.Read();
  }
  commandport.Write(0xAE); // activate interrupts for keyboard
  commandport.Write(0x20); // get current state

  uint8_t status = (dataport.Read() | 1) & ~0x10;
  commandport.Write(0x60); // set state
  dataport.Write(status);
  dataport.Write(0xF4);
  printf("Keyboard driver constructed\n");
}
KeyboardDriver::~KeyboardDriver() {}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp) {
  uint8_t key = dataport.Read();

  printf("KeyboardDriver::HandleInterrupt\n");

  char *foo = "KEYBOARD 0x00";
  char *hex = "0123456789ABCDEF";
  foo[11] = hex[(key >> 4) & 0x0F];
  foo[12] = hex[key & 0x0F];
  printf(foo);

  return esp;
}