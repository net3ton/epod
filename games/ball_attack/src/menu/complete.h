#ifndef __MENU_COMPLETE_H__
#define __MENU_COMPLETE_H__

#include "../uiMenuPopup.h"
#include "../ui/buttonMenu.h"
#include "../ui/textMenu.h"


class MenuComplete:
  public MenuPopup
{
MENU_SINGLETON_DEF(MenuComplete)

public:
  MenuComplete();

  virtual void init();

private:
  virtual void onAct(UIControl *ctrl);
  virtual void onExit(int reason);

  TextMenu txtComplete;
  ButtonTile btnLevels, btnRetry, btnNext;

  TextMenu txtScoresTxt, txtScoresVal;
  TextMenu txtLivesTxt, txtLivesVal;
  TextMenu txtBonusesTxt, txtBonusesVal;
  TextMenu txtTimeTxt, txtTimeVal;
};

#endif //__MENU_COMPLETE_H__
