#include "hud.h"
#include "pause.h"
#include "../res.h"
#include "../levelBase.h"
#include "../levelStats.h"

MENU_SINGLETON_IMPLE(MenuHud)

enum
{
  BASE_SCORES_SPEED = 2,
  MENU_HEIGHT = 39,
};


MenuHud::MenuHud()
  :m_level(NULL)
  ,btnPause(this, 290, 4, &::res.mTiles.icon_pause)
  ,txtScores(this, 7, 18, "", ep::A_LCENTER)
  ,txtLives(this, 255, 18, "", ep::A_LCENTER)
  ,m_livesPos(240, 20)
  ,m_curScores(0), m_outScores(-1)
  ,m_curLives(-1)
  ,m_menuHeight(0)
  ,m_menuAlpha(0)
{
}


void MenuHud::reset()
{
  m_outScores = m_curScores = ba::stats().getScores();
  txtScores.setText("%d", m_outScores);  
}


void MenuHud::anim(bool in)
{
  m_animTimer.reverce(in);
  m_animTimer.start(MENU_ANIM_TIME);
}


void MenuHud::render(ep::IGraphics *graph)
{
  graph->drawFillRect(0, 0, 320, MENU_HEIGHT, 0xFF220055);
  graph->drawTile(0, MENU_HEIGHT, &::res.mTiles.line_bottom, 0xFFFFFFFF, ep::A_LEFT_BOTTOM);

  UIMenu::render(graph);

  graph->drawTile(m_livesPos.x, m_livesPos.y, &::res.mTiles.ball, m_menuAlpha | 0x00FFFFFF, ep::A_CENTER);
}


void MenuHud::update(long dt)
{
  UIMenu::update(dt);

  m_animTimer.update(dt);

  if (m_animTimer.isWorking())
  {
    //m_menuHeight = MENU_HEIGHT * cos(m_animTimer.getVal(HALFPI));
    m_menuAlpha = m_animTimer.getInvVal(255) << 24;

    for (int i=0; i<m_controls.size(); ++i)
      m_controls[i]->setColor(m_menuAlpha | 0x00FFFFFF);
  }

  int needScores = ba::stats().getScores();

  if (m_curScores != needScores)
  {
    int dif = needScores - m_curScores;
    int delta = BASE_SCORES_SPEED;
    if (dif < delta)
    {
      delta = dif;
    }
    else
    {
      int time = dif / BASE_SCORES_SPEED;
      if (time > 5)
        delta = dif / 5;
    }

    m_curScores += delta;
  }

  if (m_outScores != m_curScores)
  {
    m_outScores = m_curScores;
    txtScores.setText("%d", m_outScores);
  }

  int needLives = ba::stats().getLives();

  if (m_curLives != needLives)
  {
    m_curLives = needLives;
    txtLives.setText("%d", ba::stats().getLives());
  }
}


void MenuHud::onAct(UIControl *ctrl)
{
  if (ctrl == &btnPause)
  {
    m_level->showPauseMenu(true);
  }
}
