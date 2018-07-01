#include "text.h"
#include "../uiMenu.h"

#include <epod_graphics.h>
#include <stdarg.h>


UIText::UIText(UIMenu *menu)
  :UIControl(menu)
{
}


void UIText::setPos(int x, int y)
{
  UIControl::setPos(x, y);
  updateTextPos();
}


void UIText::setSize(int w, int h)
{
  UIControl::setSize(w, h);
  updateTextPos();
}


void UIText::setColor(int color)
{
  UIControl::setColor(color);
  m_textRenderer.setColor(color);
}


void UIText::setAlign(int align)
{
  m_textRenderer.setAlign(align);
  updateTextPos();
}


void UIText::updateTextPos()
{
  float x = m_pos.x;
  float y = m_pos.y;

  int align = m_textRenderer.getAlign();

  if (align & ep::A_RIGHT)
    x += m_size.x;
  else if (align & ep::A_HCENTER)
    x += m_size.x/2;

  if (align & ep::A_BOTTOM)
    y += m_size.y;
  else if (align & ep::A_VCENTER)
    y += m_size.y/2;

  m_textRenderer.setPos(x, y);
}


void UIText::render(ep::IGraphics *graph)
{
  m_textRenderer.draw(graph);
}


void UIText::setText(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  m_textRenderer.printfa(fmt, ap);

  va_end(ap);
}
