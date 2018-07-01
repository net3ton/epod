#ifndef __MENU_HELP_H__
#define __MENU_HELP_H__

#include "../uiMenu.h"
#include "../ui/buttonMenu.h"
#include "../ui/textMenu.h"
#include "../bonuses.h"

#include <epod_font.h>
#include <timer_int.h>


class MenuHelp:
  public UIMenu
{
MENU_SINGLETON_DEF(MenuHelp)

public:
  MenuHelp();

  virtual void update(long dt);
  virtual void render(ep::IGraphics *graph);
  virtual void onAct(UIControl *ctrl);

  void anim(bool in);

private:
  ButtonTile btnBack;
  TextMenu txtName;

  struct BonusInfo
  {
    const ep::Tile *icon;
    ep::TextRenderer descr;
  };

  BonusInfo m_info[BONUSES_COUNT];

  TimerInt m_animTimer;
  int m_menuHeight, m_topHeight;
  int m_menuColor;
};


#endif //__MENU_HELP_H__
