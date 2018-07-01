#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "../uiControl.h"
#include <epod_font.h>
#include <epod.h>


class UIText
  :public UIControl
{
public:
  UIText(UIMenu *menu);

  virtual void render(ep::IGraphics *graph);

  virtual void setPos(int x, int y);
  virtual void setSize(int w, int h);
  virtual void setColor(int color);

  void setText(const char *fmt, ...);
  void setAlign(int align);

protected:
  void setFont(ep::Font *font) { m_textRenderer.setFont(font); }
  void updateTextPos();

  ep::TextRenderer m_textRenderer;
};

#endif //__UI_TEXT_H__
