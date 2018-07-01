#include "help.h"
#include "pause.h"
#include "../main.h"
#include "../bonusManager.h"
#include "../res.h"

MENU_SINGLETON_IMPLE(MenuHelp)

enum
{
  TOP_OFFSET = 70,
  LINE_HEIGHT = 32,
};


MenuHelp::MenuHelp()
  :btnBack(this, 5, 425, &::res.mTiles.icon_back)
  ,txtName(this, 160, 20, "Bonuses", ep::A_CENTER)
  ,m_menuHeight(0), m_topHeight(0)
  ,m_menuColor(0)
{
  for (int id=0; id<BONUSES_COUNT; ++id)
  {
    m_info[id].icon = get_bonus_drop_tile(id);
    m_info[id].descr.setFont(&res.mFonts.text);
    m_info[id].descr.setAlign(ep::A_LCENTER);
    m_info[id].descr.setPos(45, TOP_OFFSET + LINE_HEIGHT * id);
    m_info[id].descr.setMaxWidth(270);
    m_info[id].descr.setStr(get_bonus_descr(id));
  }
}


void MenuHelp::anim(bool in)
{
  lockControls(!in);

  m_animTimer.reverce(in);
  m_animTimer.start(MENU_ANIM_TIME);
}


void MenuHelp::update(long dt)
{
  UIMenu::update(dt);

  m_animTimer.update(dt);

  if (m_animTimer.isWorking())
  {
    m_menuHeight = 220 * cos(m_animTimer.getVal(HALFPI));
    m_topHeight = 39 * cos(m_animTimer.getVal(HALFPI));
    m_menuColor = (m_animTimer.getInvVal(255) << 24) | 0x00FFFFFF;

    txtName.setColor(m_menuColor);
    btnBack.setColor(m_menuColor);

    for (int id=0; id<BONUSES_COUNT; ++id)
      m_info[id].descr.setColor(m_menuColor);
  }
}


void MenuHelp::render(ep::IGraphics *graph)
{
  graph->drawFillRect(0, 0, 320, m_topHeight, 0xFF150035);
  graph->drawTile(0, m_topHeight, &::res.mTiles.line_bottom, m_menuColor, ep::A_LEFT_BOTTOM);

  const int posCenter = 260;

  int posTop = posCenter - m_menuHeight;
  int posBottom = posCenter + m_menuHeight;

  graph->drawFillRect(0, posTop, 320, m_menuHeight * 2, 0xD0150035);
  graph->drawTile(0, posTop, &::res.mTiles.line_top, m_menuColor);
  graph->drawTile(0, posBottom, &::res.mTiles.line_bottom, m_menuColor, ep::A_LEFT_BOTTOM);

  UIMenu::render(graph);

  for (int id=0; id<BONUSES_COUNT; ++id)
  {
    graph->drawTile(25, TOP_OFFSET + LINE_HEIGHT * id, m_info[id].icon, m_menuColor, ep::A_CENTER);
    m_info[id].descr.draw(graph);
  }
}


void MenuHelp::onAct(UIControl *ctrl)
{
  if (ctrl == &btnBack)
    MenuPause::getMenu()->showHelp(false);
}
