#ifndef __MENU_LEVELS_H__
#define __MENU_LEVELS_H__

#include "../uiMenu.h"
#include "../ui/buttonMenu.h"
#include "../ui/levelSlider.h"
#include "../ui/textMenu.h"

#include <epod_font.h>


class MenuLevels:
  public UIMenu
{
MENU_SINGLETON_DEF(MenuLevels)

public:
  MenuLevels();

  virtual void init();
  virtual void onAct(UIControl *ctrl);
  virtual bool onPointerDown(int idx, int x, int y);

private:
  void setHardLevels(bool hard);
  void updateLevels();

  ButtonTile btnBack;
  ButtonCheck btnNormal, btnHard;
  LevelsSlider levels;
  TextMenu txtStars;
};


#endif //__MENU_LEVELS_H__
