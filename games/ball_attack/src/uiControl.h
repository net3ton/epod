#ifndef __UI_CONTROL_H__
#define __UI_CONTROL_H__

#include <math/point2.h>

namespace ep { class IGraphics; }
class UIMenu;


class UIControl
{
public:
  UIControl(UIMenu *menu);
  virtual ~UIControl() {}

  virtual void init() {}
  virtual void destroy() {}

  virtual void update(long dt) {}
  virtual void render(ep::IGraphics *graph) {}

  virtual bool hitTest(int x, int y);

  virtual void onPointerDown(int x, int y) {}
  virtual void onPointerUp(int x, int y) {}
  virtual void onPointerMove(int x, int y) {}

  virtual void onFocus(bool focus) {}

  ///

  virtual void setPos(int x, int y) { m_pos.set(x, y); }
  virtual void setSize(int w, int h) { m_size.set(w, h); }
  virtual void setColor(int color) { m_color = color; }

  bool isVisible() const { return m_visible; }
  bool isEnabled() const { return m_enabled; }

  void setVisible(bool visible) { m_visible = visible; }
  void setEnabled(bool enabled) { m_enabled = enabled; }

protected:
  UIMenu *m_menu;
  bool m_visible, m_enabled;
  Point2i m_pos;  // left-top
  Point2i m_size;
  int m_color;
};

#endif //__UI_CONTROL_H__
