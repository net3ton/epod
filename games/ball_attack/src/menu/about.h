#ifndef __MENU_ABOUT_H__
#define __MENU_ABOUT_H__

#include "../uiMenu.h"
#include "../ui/buttonMenu.h"
#include "../ui/textMenu.h"


class MenuAbout:
  public UIMenu
{
MENU_SINGLETON_DEF(MenuAbout)

public:
  MenuAbout();

  virtual void init();
  virtual void onAct(UIControl *ctrl);
  virtual bool onPointerDown(int idx, int x, int y);

private:
  ButtonTile btnBack;
  TextMenu txtName;
};


#endif //__MENU_ABOUT_H__
