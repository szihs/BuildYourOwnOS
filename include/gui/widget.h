#pragma once
#include <common/graphicscontext.h>
#include <common/types.h>

namespace os {
namespace gui {
class Widget {
private:
  bool isFocussable;
  virtual void doGetFocus(Widget *widget);
  virtual void doModelToScreen(common::int32_t &x, common::int32_t &y);

  virtual void doDraw(common::GraphicsContext *gc);
  virtual void doOnMouseDown(common::int32_t x, common::int32_t y);
  virtual void doOnMouseUp(common::int32_t x, common::int32_t y);
  virtual void doOnMouseMove(common::int32_t oldx, common::int32_t oldy,
                             common::int32_t newx, common::int32_t newy);

  virtual void doOnKeyDown(char c);
  virtual void doOnKeyUp(char c);

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
  void OnMouseDown(common::int32_t x, common::int32_t y);
  void OnMouseUp(common::int32_t x, common::int32_t y);
  void OnMouseMove(common::int32_t oldx, common::int32_t oldy,
                   common::int32_t newx, common::int32_t newy);

  void OnKeyDown(char c);
  void OnKeyUp(char c);
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

  virtual void doDraw(common::GraphicsContext *gc);
  virtual void doOnMouseDown(common::int32_t x, common::int32_t y);
  virtual void doOnMouseUp(common::int32_t x, common::int32_t y);
  virtual void doOnMouseMove(common::int32_t oldx, common::int32_t oldy,
                             common::int32_t newx, common::int32_t newy);

  virtual void doOnKeyDown(char c);
  virtual void doOnKeyUp(char c);

public:
  CompositeWidget(Widget *parent, common::int32_t x, common::int32_t y,
                  common::int32_t w, common::int32_t h, common::uint8_t r,
                  common::uint8_t g, common::uint8_t b);
  ~CompositeWidget();
};
} // namespace gui
} // namespace os