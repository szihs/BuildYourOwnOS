#include <drivers/mouse.h>
using namespace os::common;
using namespace os::drivers;
using namespace os::hardwarecomm;
void printf(const char *str);

void MouseEventHandler::doOnMouseDown(uint8_t button) {}
void MouseEventHandler::doOnMouseUp(uint8_t button) {}
void MouseEventHandler::doOnMouseMove(int8_t x, int8_t y) {}
void MouseEventHandler::doOnActivate() {}

void MouseEventHandler::OnMouseDown(uint8_t button) {
  return doOnMouseDown(button);
}
void MouseEventHandler::OnMouseUp(uint8_t button) {
  return doOnMouseUp(button);
}
void MouseEventHandler::OnMouseMove(int8_t x, int8_t y) {
  return doOnMouseMove(x, y);
}
void MouseEventHandler::OnActivate() { return doOnActivate(); }

MouseDriver::MouseDriver(InterruptManager *manager, MouseEventHandler *handler)
    : InterruptHandler(0x2C, manager), dataport(0x60), commandport(0x64) {
  printf("Mouse driver constructed\n");
  this->handler = handler;
}
MouseDriver::~MouseDriver() { printf("mouse destructor\n"); }

void MouseDriver::doActivate() {
  printf("Activate mouse\n");
  offset = 0;
  buttons = 0;

  commandport.Write(0xA8); // activate interrupts for mouse
  commandport.Write(0x20); // get current state

  uint8_t status = (dataport.Read() | 2);
  commandport.Write(0x60); // set state
  dataport.Write(status);

  commandport.Write(0xD4);
  dataport.Write(0xF4);
  dataport.Read();
}

uint32_t MouseDriver::doHandleInterrupt(uint32_t esp) {

  uint8_t status = commandport.Read();
  if (!(status & 0x20)) {
    return esp;
  }
  buffer[offset] = dataport.Read();

  if (!handler)
    return esp;

  offset = (offset + 1) % 3;

  if (offset == 0) {
    handler->OnMouseMove(buffer[1], -buffer[2]);

    for (uint8_t i = 0; i < 3; i++) {
      if ((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i))) {
        if (buttons & (0x01 << i))
          handler->OnMouseUp(i + 1);
        else
          handler->OnMouseDown(i + 1);
      }
    }
    buttons = buffer[0];
  }

  return esp;
}
