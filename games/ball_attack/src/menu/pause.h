#ifndef __MENU_PAUSE_H__
#define __MENU_PAUSE_H__

#include "../uiMenu.h"
#include "../ui/buttonMenu.h"
#include "../ui/textMenu.h"

#include <timer_int.h>


class MenuHelp;

class MenuPause:
  public UIMenu
{
MENU_SINGLETON_DEF(MenuPause)

public:
  MenuPause();

  virtual void init();

  virtual void update(long dt);
  virtual void render(ep::IGraphics *graph);

  virtual void onAct(UIControl *ctrl);

  virtual bool onPointerDown(int idx, int x, int y);
  virtual bool onPointerUp(int idx, int x, int y);
  virtual bool onPointerMove(int idx, int x, int y);

  void showHelp(bool show);

private:
  void anim(bool in);

  ButtonTile btnResume, btnHelp;
  ButtonTileTimer btnRestart, btnLevels;
  ButtonCheck btnSound, btnMusic;
  TextMenu txtPause;

  MenuHelp *m_menuHelp;

  TimerInt m_animTimer;
  int m_animHeight;
};


#endif //__MENU_PAUSE_H__
