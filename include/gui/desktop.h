#pragma once
#include <common/types.h>
#include <drivers/mouse.h>
#include <gui/widget.h>

namespace os {
namespace gui {
class Desktop : public CompositeWidget, public drivers::MouseEventHandler {
private:
  common::uint32_t MouseX;
  common::uint32_t MouseY;

  virtual void doOnMouseDown(os::common::uint8_t button);
  virtual void doOnMouseUp(os::common::uint8_t button);
  virtual void doOnMouseMove(os::common::int8_t x, os::common::int8_t y);

public:
  Desktop(common::int32_t w, common::int32_t h, common::uint8_t r,
          common::uint8_t g, common::uint8_t b);
  ~Desktop();
  void Draw(common::GraphicsContext *gc);
};
} // namespace gui
} // namespace os