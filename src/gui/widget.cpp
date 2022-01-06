#include <gui/widget.h>
using namespace os::common;
using namespace os::gui;

Widget::Widget(Widget *parent, int32_t x, int32_t y, int32_t w, int32_t h,
               uint8_t r, uint8_t g, uint8_t b) {

  this->parent = parent;
  this->x = x;
  this->y = y;
  this->w = w;
  this->y = y;
  this->r = r;
  this->g = g;
  this->b = b;
  this->isFocussable = true;
}
Widget::~Widget() {}

void Widget::doGetFocus(Widget *widget) {
  if (parent)
    parent->doGetFocus(widget); // default impl
}
void Widget::doModelToScreen(int32_t &x, int32_t &y) {
  if (parent)
    parent->doModelToScreen(x, y);

  x += this->x;
  y += this->y;
}

void Widget::doDraw(GraphicsContext *gc) {
  int X = 0;
  int Y = 0;
  ModelToScreen(X, Y);
  gc->FillRectangle(X, Y, w, h, r, g, b);
}
void Widget::doOnMouseDown(int32_t x, int32_t y) {
  if (isFocussable)
    GetFocus(this);
}
void Widget::doOnMouseUp(int32_t x, int32_t y) {}
void Widget::doOnMouseMove(int32_t oldx, int32_t oldy, int32_t newx,
                           int32_t newy) {}

void Widget::doOnKeyDown(char c) {}
void Widget::doOnKeyUp(char c) {}

void Widget::GetFocus(Widget *widget) { return doGetFocus(widget); }
void Widget::ModelToScreen(int32_t &x, int32_t &y) {
  return doModelToScreen(x, y);
}
bool Widget::ContainsCoordinate(int32_t x, int32_t y) { return true; }

void Widget::Draw(GraphicsContext *gc) { return doDraw(gc); }
void Widget::OnMouseDown(int32_t x, int32_t y) { return doOnMouseDown(x, y); }
void Widget::OnMouseUp(int32_t x, int32_t y) { return doOnMouseUp(x, y); }
void Widget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx,
                         int32_t newy) {
  return doOnMouseMove(oldx, oldy, newx, newy);
}

void Widget::OnKeyDown(char c) { return doOnKeyDown(c); }
void Widget::OnKeyUp(char c) { return doOnKeyUp(c); }

CompositeWidget::CompositeWidget(Widget *parent, int32_t x, int32_t y,
                                 int32_t w, int32_t h, uint8_t r, uint8_t g,
                                 uint8_t b)
    : Widget(parent, x, y, w, h, r, g, b) {
  this->numChildren = 0;
  this->focussedChild = nullptr;
}
CompositeWidget::~CompositeWidget() {}

void CompositeWidget::doGetFocus(Widget *widget) {
  this->focussedChild = widget;
  if (parent)
    parent->GetFocus(widget);
}

void CompositeWidget::doDraw(GraphicsContext *gc) {
  Widget::Draw(gc);
  for (int32_t i = numChildren; i >= 0; i--)
    child[i]->Draw(gc);
}

// index 0 is topmost in z order

void CompositeWidget::doOnMouseDown(int32_t x, int32_t y) {
  for (int32_t i = 0; i < numChildren; i++) {
    if (child[i]->ContainsCoordinate(x - this->x, y - this->y)) {
      child[i]->OnMouseDown(x - this->x, y - this->y);
      break;
    }
  }
}
void CompositeWidget::doOnMouseUp(int32_t x, int32_t y) {
  for (int32_t i = 0; i < numChildren; i++) {
    if (child[i]->ContainsCoordinate(x - this->x, y - this->y)) {
      child[i]->OnMouseUp(x - this->x, y - this->y);
      break;
    }
  }
}
void CompositeWidget::doOnMouseMove(int32_t oldx, int32_t oldy, int32_t newx,
                                    int32_t newy) {
  int32_t firstChild = -1;
  for (int32_t i = 0; i < numChildren; i++) {
    if (child[i]->ContainsCoordinate(oldx - this->x, oldy - this->y)) {
      child[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x,
                            newy - this->y);
      firstChild = i;
      break;
    }
  }

  for (int32_t i = 0; i < numChildren; i++) {
    if (child[i]->ContainsCoordinate(newx - this->x, newy - this->y)) {
      if (firstChild != i)
        child[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x,
                              newy - this->y);
      break;
    }
  }
}

void CompositeWidget::doOnKeyDown(char c) {
  if (focussedChild)
    focussedChild->OnKeyDown(c);
}
void CompositeWidget::doOnKeyUp(char c) {

  if (focussedChild)
    focussedChild->OnKeyUp(c);
}
