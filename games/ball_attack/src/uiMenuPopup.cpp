#include "uiMenuPopup.h"
#include "uiControl.h"
#include "res.h"


MenuPopup::MenuPopup(int height, int cpos)
  :HEIGHT(height)
  ,CPOS(cpos)
  ,m_menuHeight(0)
  ,m_menuColor(0)
  ,m_exitReason(0)
{
}


void MenuPopup::init()
{
  UIMenu::init();
  anim(true);
}


void MenuPopup::exitMenu(int reason)
{
  m_exitReason = reason;
  anim(false);
}


void MenuPopup::anim(bool in)
{
  lockControls(!in);

  m_animTimer.reverce(in);
  m_animTimer.start(MENU_ANIM_TIME);
}

void MenuPopup::update(long dt)
{
  UIMenu::update(dt);

  m_animTimer.update(dt);

  if (m_animTimer.isWorking())
  {
    m_menuHeight = HEIGHT * cos(m_animTimer.getVal(HALFPI));
    m_menuColor = (m_animTimer.getInvVal(255) << 24) | 0x00FFFFFF;

    for (int i=0; i<m_controls.size(); ++i)
      m_controls[i]->setColor(m_menuColor);
  }

  if (m_animTimer.isDirectEnd())
    onExit(m_exitReason);
}


void MenuPopup::render(ep::IGraphics *graph)
{
  graph->drawFillRect(0, 39, 320, 480, 0x60000000);

  int posTop = CPOS - m_menuHeight;
  int posBottom = CPOS + m_menuHeight;

  graph->drawFillRect(0, posTop, 320, m_menuHeight * 2, 0xD0150035);
  graph->drawTile(0, posTop, &::res.mTiles.line_top, m_menuColor);
  graph->drawTile(0, posBottom, &::res.mTiles.line_bottom, m_menuColor, ep::A_LEFT_BOTTOM);

  UIMenu::render(graph);
}
