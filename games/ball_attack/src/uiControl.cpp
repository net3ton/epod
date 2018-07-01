#include "uiControl.h"
#include "uiMenu.h"


UIControl::UIControl(UIMenu *menu)
  :m_menu(menu), m_visible(true), m_enabled(true), m_color(0xFFFFFFFF)
{
  if (m_menu)
    m_menu->addControl(this);
}

bool UIControl::hitTest(int x, int y)
{
  return x >= m_pos.x && x <= (m_pos.x + m_size.x) && y >= m_pos.y && y <= (m_pos.y + m_size.y);
}
