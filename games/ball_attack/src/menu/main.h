#ifndef __MENU_MAIN_H__
#define __MENU_MAIN_H__

#include "../uiMenu.h"
#include "../ui/buttonMenu.h"

#include <vector>


class MenuMain:
  public UIMenu
{
MENU_SINGLETON_DEF(MenuMain)

public:
  MenuMain();

  virtual void init();
  virtual void update(long dt);
  virtual void onAct(UIControl *ctrl);

private:
  ButtonMenu btnAction, btnSurvival;
  ButtonTile btnLeaders, btnAchieves, btnOf;
  ButtonTile btnAbout;
  ButtonTile btnEditor;
  ButtonCheck btnSound, btnMusic;
};


#endif //__MENU_MAIN_H__
