#ifndef __MENU_HUD_H__
#define __MENU_HUD_H__

#include "../uiMenu.h"
#include "../ui/buttonMenu.h"
#include "../ui/textMenu.h"

#include <timer_int.h>

class LevelBase;


class MenuHud:
  public UIMenu
{
MENU_SINGLETON_DEF(MenuHud)

public:
  MenuHud();

  virtual void update(long dt);
  virtual void render(ep::IGraphics *graph);
  virtual void onAct(UIControl *ctrl);

  void reset();

  void setLevel(LevelBase *level) { m_level = level; }
  void showPauseButton(bool show) { btnPause.setVisible(show); }

  const Point2f &getLivesPos() { return m_livesPos; }

  void anim(bool in);

private:
  ButtonHud btnPause;
  TextMenu txtScores, txtLives;

  LevelBase *m_level;

  int m_curScores, m_outScores;
  int m_curLives;
  Point2f m_livesPos;

  TimerInt m_animTimer;
  int m_menuHeight;
  int m_menuAlpha;
};


#endif //__MENU_HUD_H__
