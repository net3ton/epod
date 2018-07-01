#include "complete.h"
#include "levels.h"
#include "../main.h"
#include "../levels.h"
#include "../levelStats.h"
#include "../res.h"

enum
{
  EXIT_LEVELS = 0,
  EXIT_RETRY,
  EXIT_NEXT,
};


MENU_SINGLETON_IMPLE(MenuComplete)

MenuComplete::MenuComplete()
  :MenuPopup(120, 180)
  ,txtComplete(this, 160, 95, "Level Complete!", ep::A_CENTER)
  ,btnLevels(this, 75, 235, &::res.mTiles.icon_levels)
  ,btnRetry(this, 135, 235, &::res.mTiles.icon_retry)
  ,btnNext(this, 195, 235, &::res.mTiles.icon_next)
  ,txtScoresTxt(this, 30, 130, "Scores:", ep::A_LCENTER)
  ,txtScoresVal(this, 230, 130, "", ep::A_LCENTER)
  ,txtLivesTxt(this, 30, 155, "Balls left:", ep::A_LCENTER)
  ,txtLivesVal(this, 230, 155, "", ep::A_LCENTER)
  ,txtBonusesTxt(this, 30, 180, "Bonuses:", ep::A_LCENTER)
  ,txtBonusesVal(this, 230, 180, "", ep::A_LCENTER)
  ,txtTimeTxt(this, 30, 205, "Time:", ep::A_LCENTER)
  ,txtTimeVal(this, 230, 205, "", ep::A_LCENTER)
{
}


void MenuComplete::init()
{
  MenuPopup::init();
  ba::complete_level(ba::stats().getLevelId());

  txtScoresVal.setText("%d", ba::stats().getScores());
  txtLivesVal.setText("%d", ba::stats().getLives());
}


void MenuComplete::onExit(int reason)
{
  if (reason == EXIT_LEVELS)
    ba::move_to_scene(MenuLevels::getMenu(), true);
  else if (reason == EXIT_RETRY)
    ba::restart_level();
  else if (reason == EXIT_NEXT)
    ba::start_next_level();
}


void MenuComplete::onAct(UIControl *ctrl)
{
  if (ctrl == &btnLevels)
    exitMenu(EXIT_LEVELS);
  else if (ctrl == &btnRetry)
    exitMenu(EXIT_RETRY);
  else if (ctrl == &btnNext)
    exitMenu(EXIT_NEXT);
}
