#pragma once

#include <drivers/mouse.h>
#include <gui/widget.h>

namespace os {
namespace gui {
class Window : public CompositeWidget {
private:
  bool draggable;

  // widget mouse override
  void doOnMouseDown(common::int32_t x, common::int32_t y,
                     common::uint8_t button);
  void doOnMouseUp(common::int32_t x, common::int32_t y,
                   common::uint8_t button);
  void doOnMouseMove(common::int32_t oldx, common::int32_t oldy,
                     common::int32_t newx, common::int32_t newy);

public:
  Window(Widget *parent, common::int32_t x, common::int32_t y,
         common::int32_t w, common::int32_t h, common::uint8_t r,
         common::uint8_t g, common::uint8_t b);
  ~Window();
};
} // namespace gui
} // namespace os