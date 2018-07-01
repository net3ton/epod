#ifndef __MENU_FAILED_H__
#define __MENU_FAILED_H__

#include "../uiMenuPopup.h"
#include "../ui/buttonMenu.h"
#include "../ui/textMenu.h"


class MenuFailed:
  public MenuPopup
{
MENU_SINGLETON_DEF(MenuFailed)

public:
  MenuFailed();

private:
  virtual void onAct(UIControl *ctrl);
  virtual void onExit(int reason);

  TextMenu txtFailed;
  ButtonTile btnLevels, btnRetry;
};


class MenuSurvivalEnd:
  public MenuPopup
{
MENU_SINGLETON_DEF(MenuSurvivalEnd)

public:
  MenuSurvivalEnd();

private:
  virtual void onAct(UIControl *ctrl);
  virtual void onExit(int reason);

  TextMenu txtTitle;
  ButtonTile btnExit, btnRetry;
};


#endif //__MENU_FAILED_H__
