#include <gui/widget.h>
using namespace os::common;
using namespace os::gui;

void printf(const char *str);
void printfHex(uint8_t key);

Widget::Widget(Widget *parent, int32_t x, int32_t y, int32_t w, int32_t h,
               uint8_t r, uint8_t g, uint8_t b)
    : KeyboardEventHandler() {

  this->parent = parent;
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
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
    parent->ModelToScreen(x, y);

  x += this->x;
  y += this->y;
}

void Widget::doOnMouseDown(int32_t x, int32_t y, uint8_t button) {
   if (isFocussable)
     GetFocus(this);
}
void Widget::doOnMouseUp(int32_t x, int32_t y, uint8_t button) {}
void Widget::doOnMouseMove(int32_t oldx, int32_t oldy, int32_t newx,
                           int32_t newy) {}

void Widget::GetFocus(Widget *widget) { return doGetFocus(widget); }
void Widget::ModelToScreen(int32_t &x, int32_t &y) {
  return doModelToScreen(x, y);
}

// are relative coordinates
bool Widget::ContainsCoordinate(int32_t x, int32_t y) {
  return (this->x <= x) && (x < (this->x + this->w)) && (this->y <= y) &&
         (y < (this->y + this->h));
}

void Widget::Draw(GraphicsContext *gc) {
  //  printf(" Widget Draw\n");
  int X = 0;
  int Y = 0;
  ModelToScreen(X, Y);
  gc->FillRectangle(X, Y, w, h, r, g, b);
}
void Widget::OnMouseDown(int32_t x, int32_t y, uint8_t button) {
  return doOnMouseDown(x, y, button);
}
void Widget::OnMouseUp(int32_t x, int32_t y, uint8_t button) {
  return doOnMouseUp(x, y, button);
}
void Widget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx,
                         int32_t newy) {
  return doOnMouseMove(oldx, oldy, newx, newy);
}
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

void CompositeWidget::Draw(GraphicsContext *gc) {
  // printf("Composite Draw\n");
  Widget::Draw(gc);
  for (int32_t i = numChildren - 1; i >= 0; i--)
    child[i]->Draw(gc);
}

// index 0 is topmost in z order

void CompositeWidget::OnMouseDown(int32_t x, int32_t y, uint8_t button) {
  printf("Composite mouse down called\n");
  for (int32_t i = 0; i < numChildren; i++) {
    if (child[i]->ContainsCoordinate(x - this->x, y - this->y)) {
      printf("child mouse down contained coord\n");
      child[i]->OnMouseDown(x - this->x, y - this->y, button);
      break;
    }
  }
}
void CompositeWidget::OnMouseUp(int32_t x, int32_t y, uint8_t button) {
  for (int32_t i = 0; i < numChildren; i++) {
    if (child[i]->ContainsCoordinate(x - this->x, y - this->y)) {
      child[i]->OnMouseUp(x - this->x, y - this->y, button);
      break;
    }
  }
}
void CompositeWidget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx,
                                  int32_t newy) {
  int32_t firstChild = -1;
  for (int32_t i = 0; i < numChildren; i++) {
    if (child[i]->ContainsCoordinate(oldx - this->x, oldy - this->y)) {
      printf("child mouse move contained coord\n");

      child[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x,
                            newy - this->y);
      firstChild = i;
      break;
    }
  }

  for (int32_t i = 0; i < numChildren; i++) {
    if (child[i]->ContainsCoordinate(newx - this->x, newy - this->y)) {
      if (firstChild != i) {
        child[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x,
                              newy - this->y);
        printf("child mouse move contained coord\n");
      }
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

bool CompositeWidget::AddChild(Widget *child) {
  if (numChildren >= 256)
    return false;

  this->child[numChildren++] = child;
  return true;
}
