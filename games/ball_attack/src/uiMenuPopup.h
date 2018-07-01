#ifndef __UI_MENU_POPUP_H__
#define __UI_MENU_POPUP_H__

#include "uiMenu.h"
#include <timer_int.h>


class MenuPopup:
  public UIMenu
{
public:
  MenuPopup(int height, int cpos);

  virtual void init();
  virtual void update(long dt);
  virtual void render(ep::IGraphics *graph);

protected:
  void exitMenu(int reason);
  void anim(bool in);

  virtual void onExit(int reason) = 0;

  TimerInt m_animTimer;
  int m_menuHeight;
  int m_menuColor;

  int m_exitReason;

  const int HEIGHT;
  const int CPOS;
};

#endif //__UI_MENU_POPUP_H__
