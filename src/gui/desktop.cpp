#include <gui/desktop.h>
using namespace os::common;
using namespace os::gui;
void printf(const char *str);
void printfHex(uint8_t key);
Desktop::Desktop(int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b)
    : CompositeWidget(nullptr, 0, 0, w, h, r, g, b), MouseEventHandler() {
  MouseX = w / 2;
  MouseY = h / 2;
}
Desktop::~Desktop() {}

void Desktop::Draw(GraphicsContext *gc) {
  printf("Draw for deskop\n");
  CompositeWidget::Draw(gc);
  for (uint8_t i = 0; i < 4; i++) {
    gc->PutPixel(MouseX + i, MouseY, 0xFF, 0xFF, 0xFF);
    gc->PutPixel(MouseX - i, MouseY, 0xFF, 0xFF, 0xFF);
    gc->PutPixel(MouseX, MouseY + i, 0xFF, 0xFF, 0xFF);
    gc->PutPixel(MouseX, MouseY - i, 0xFF, 0xFF, 0xFF);
  }
}

void Desktop::doOnMouseDown(uint8_t button) {
  CompositeWidget::OnMouseDown(MouseX, MouseY, button);
}
void Desktop::doOnMouseUp(uint8_t button) {
  CompositeWidget::OnMouseUp(MouseX, MouseY, button);
}
void Desktop::doOnMouseMove(int8_t x, int8_t y) {
  x /= 4;
  y /= 4;

  int16_t newMouseX = MouseX + x;
  int16_t newMouseY = MouseY + y;

  if (newMouseX < 0)
    newMouseX = 0;
  if (newMouseX >= w) {
    newMouseX = w - 1;
  }

  if (newMouseY < 0)
    newMouseY = 0;
  if (newMouseY >= h) {
    newMouseY = h - 1;
  }

  CompositeWidget::OnMouseMove(MouseX, MouseY, newMouseX, newMouseY);
  MouseX = newMouseX;
  MouseY = newMouseY;
}