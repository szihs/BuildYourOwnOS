#pragma once
#include <common/graphicscontext.h>
#include <common/types.h>
#include <drivers/keyboard.h>

namespace os {
namespace gui {
class Widget : public drivers::KeyboardEventHandler {
private:
  bool isFocussable;
  virtual void doGetFocus(Widget *widget);
  virtual void doModelToScreen(common::int32_t &x, common::int32_t &y);
  // override in composite/window
  virtual void doOnMouseDown(common::int32_t x, common::int32_t y,
                             common::uint8_t button);
  virtual void doOnMouseUp(common::int32_t x, common::int32_t y,
                           common::uint8_t button);
  virtual void doOnMouseMove(common::int32_t oldx, common::int32_t oldy,
                             common::int32_t newx, common::int32_t newy);

protected:
  Widget *parent;
  common::int32_t x;
  common::int32_t y;
  common::int32_t w;
  common::int32_t h;

  common::uint8_t r;
  common::uint8_t g;
  common::uint8_t b;

public:
  Widget(Widget *parent, common::int32_t x, common::int32_t y,
         common::int32_t w, common::int32_t h, common::uint8_t r,
         common::uint8_t g, common::uint8_t b);
  ~Widget();
  void GetFocus(Widget *widget);
  void ModelToScreen(common::int32_t &x, common::int32_t &y);
  bool ContainsCoordinate(common::int32_t x, common::int32_t y);
  void Draw(common::GraphicsContext *gc);

  void OnMouseDown(common::int32_t x, common::int32_t y,
                   common::uint8_t button);
  void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
  void OnMouseMove(common::int32_t oldx, common::int32_t oldy,
                   common::int32_t newx, common::int32_t newy);
};

class CompositeWidget : public Widget {
private:
  Widget *child[256];
  int numChildren;
  Widget *focussedChild;

  // override
  virtual void doGetFocus(Widget *widget);
  //  virtual void doModelToScreen(common::int32_t &x, common::int32_t &y);
  //  //not reqd

  virtual void doOnKeyDown(char c);
  virtual void doOnKeyUp(char c);

public:
  CompositeWidget(Widget *parent, common::int32_t x, common::int32_t y,
                  common::int32_t w, common::int32_t h, common::uint8_t r,
                  common::uint8_t g, common::uint8_t b);
  ~CompositeWidget();
  void Draw(common::GraphicsContext *gc);
  bool AddChild(Widget *child);
  void OnMouseDown(common::int32_t x, common::int32_t y,
                   common::uint8_t button);
  void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
  void OnMouseMove(common::int32_t oldx, common::int32_t oldy,
                   common::int32_t newx, common::int32_t newy);
};
} // namespace gui
} // namespace os
