#include "failed.h"
#include "levels.h"
#include "main.h"
#include "../main.h"
#include "../levels.h"
#include "../res.h"


enum
{
  EXIT_LEVELS = 0,
  EXIT_RETRY,
  EXIT_MAIN,
};


MENU_SINGLETON_IMPLE(MenuFailed)

MenuFailed::MenuFailed()
  :MenuPopup(90, 150)
  ,txtFailed(this, 160, 95, "Level Failed", ep::A_CENTER)
  ,btnLevels(this, 75, 175, &::res.mTiles.icon_levels)
  ,btnRetry(this, 195, 175, &::res.mTiles.icon_retry)
{
}


void MenuFailed::onExit(int reason)
{
  if (reason == EXIT_LEVELS)
    ba::move_to_scene(MenuLevels::getMenu(), true);
  else if (reason == EXIT_RETRY)
    ba::restart_level();
}


void MenuFailed::onAct(UIControl *ctrl)
{
  if (ctrl == &btnLevels)
    exitMenu(EXIT_LEVELS);
  else if (ctrl == &btnRetry)
    exitMenu(EXIT_RETRY);
}


///////////////////////////////////////////////////////////////////////////////


MENU_SINGLETON_IMPLE(MenuSurvivalEnd)

MenuSurvivalEnd::MenuSurvivalEnd()
  :MenuPopup(90, 150)
  ,txtTitle(this, 160, 95, "Level End", ep::A_CENTER)
  ,btnExit(this, 75, 175, &::res.mTiles.icon_levels)
  ,btnRetry(this, 195, 175, &::res.mTiles.icon_retry)
{
}


void MenuSurvivalEnd::onExit(int reason)
{
  if (reason == EXIT_MAIN)
    ba::move_to_scene(MenuMain::getMenu(), true);
  else if (reason == EXIT_RETRY)
    ba::restart_level();
}


void MenuSurvivalEnd::onAct(UIControl *ctrl)
{
  if (ctrl == &btnExit)
    exitMenu(EXIT_MAIN);
  else if (ctrl == &btnRetry)
    exitMenu(EXIT_RETRY);
}
