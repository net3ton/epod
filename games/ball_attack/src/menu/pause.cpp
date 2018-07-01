#include "pause.h"
#include "levels.h"
#include "main.h"
#include "help.h"
#include "../main.h"
#include "../res.h"
#include "../levelBase.h"
#include "../levelStats.h"
#include "../levels.h"
#include "../profile.h"

MENU_SINGLETON_IMPLE(MenuPause)

enum
{
  MENU_HEIGHT = 130,
};


MenuPause::MenuPause()
  :btnResume(this, 135, 365 + MENU_HEIGHT, &::res.mTiles.icon_down)
  ,btnLevels(this, 5, 425 + MENU_HEIGHT, &::res.mTiles.icon_levels)
  ,btnRestart(this, 55, 425 + MENU_HEIGHT, &::res.mTiles.icon_retry)
  ,btnHelp(this, 135, 425 + MENU_HEIGHT, &::res.mTiles.icon_help)
  ,btnSound(this, 215, 425 + MENU_HEIGHT, &::res.mTiles.icon_sound)
  ,btnMusic(this, 265, 425 + MENU_HEIGHT, &::res.mTiles.icon_music)
  ,txtPause(this, 160, 330 + MENU_HEIGHT, "PAUSE", ep::A_CENTER)
  ,m_menuHelp(0)
  ,m_animHeight(0)
{
}


void MenuPause::init()
{
  UIMenu::init();

  btnSound.setChecked(pf::is_sound());
  btnMusic.setChecked(pf::is_music());

  anim(true);
}


void MenuPause::onAct(UIControl *ctrl)
{
  if (ctrl == &btnResume)
  {
    anim(false);
  }
  else if (ctrl == &btnRestart)
  {
    ba::restart_level();
  }
  else if (ctrl == &btnLevels)
  {
    ba::level()->showPauseMenu(false);

    if (ba::is_survival())
      ba::move_to_scene(MenuMain::getMenu(), true);
    else
      ba::move_to_scene(MenuLevels::getMenu(), true);
  }
  else if (ctrl == &btnMusic)
  {
    pf::set_music(btnMusic.isChecked());
  }
  else if (ctrl == &btnSound)
  {
    pf::set_sound(btnSound.isChecked());
  }
  else if (ctrl == &btnHelp)
  {
    m_menuHelp = MenuHelp::getMenu();
    m_menuHelp->init();

    showHelp(true);
  }
}


void MenuPause::showHelp(bool show)
{
  MenuHelp::getMenu()->anim(show);
  anim(!show);
}


void MenuPause::anim(bool in)
{
  lockControls(!in);

  m_animTimer.reverce(in);
  m_animTimer.start(MENU_ANIM_TIME);
}


void MenuPause::update(long dt)
{
  if (m_menuHelp)
    m_menuHelp->update(dt);

  m_animTimer.update(dt);

  if (m_animTimer.isWorking())
  {
    m_animHeight = MENU_HEIGHT * cos(m_animTimer.getVal(HALFPI));
    txtPause.setColor(m_animTimer.getInvVal(255) << 24 | 0x00FFFFFF);
  }

  if (m_animTimer.isDirectEnd())
  {
    if (!m_menuHelp)
      ba::level()->showPauseMenu(false);
  }

  if (m_animTimer.isReverceEnd())
  {
    if (m_menuHelp)
    {
      m_menuHelp->destroy();
      m_menuHelp = 0;
    }
  }

  UIMenu::update(dt);
}


void MenuPause::render(ep::IGraphics *graph)
{
  graph->drawFillRect(0, 0, ep::get_width(), ep::get_height(), 0x60000000);

  int curPos = ep::get_height() - m_animHeight;

  graph->drawFillRect(0, curPos, 320, m_animHeight, 0xD0150035);
  graph->drawTile(0, curPos, &::res.mTiles.line_top);

  graph->wtmTrans(0, -m_animHeight);
  UIMenu::render(graph);
  graph->wtmIdent();

  if (m_menuHelp)
    m_menuHelp->render(graph);
}


bool MenuPause::onPointerUp(int idx, int x, int y)
{
  if (m_menuHelp)
    return m_menuHelp->onPointerUp(idx, x, y);

  /////////////////////////////////////////////////////////////////////////////
  if (ba::is_cheats())
  {
    if (x < 100 && y < 50)
    {
      ba::level()->completeLevel(true);
      return true;
    }

    if ((x > ep::get_width() - 100 && y < 50) && !ba::is_survival())
    {
      ba::level()->completeLevel(false);
      return true;
    }
  }
  /////////////////////////////////////////////////////////////////////////////

  return UIMenu::onPointerUp(idx, x, y + m_animHeight);
}


bool MenuPause::onPointerDown(int idx, int x, int y)
{
  if (m_menuHelp)
    return m_menuHelp->onPointerDown(idx, x, y);

  return UIMenu::onPointerDown(idx, x, y + m_animHeight);
}


bool MenuPause::onPointerMove(int idx, int x, int y)
{
  if (m_menuHelp)
    return m_menuHelp->onPointerMove(idx, x, y);

  return UIMenu::onPointerMove(idx, x, y + m_animHeight);
}
