#include "levels.h"
#include "main.h"
#include "../main.h"
#include "../levels.h"
#include "../profile.h"
#include "../res.h"

MENU_SINGLETON_IMPLE(MenuLevels)

MenuLevels::MenuLevels()
  :btnBack(this, 5, 425, &::res.mTiles.icon_back)
  ,btnNormal(this, 215, 425, &::res.mTiles.icon_normal)
  ,btnHard(this, 265, 425, &::res.mTiles.icon_hard)
  ,txtStars(this, 310, 18, "", ep::A_RCENTER)
  ,levels(this)
{
}


void MenuLevels::init()
{
  UIMenu::init();

  updateLevels();
}


void MenuLevels::updateLevels()
{
  int starsCount = 0;
  int starsMax = 0;
  levels.getStars(starsMax, starsCount);

  txtStars.setText("%d / %d", starsCount, starsMax);

  btnNormal.setChecked(!pf::is_levels_hard());
  btnHard.setChecked(pf::is_levels_hard());
}


void MenuLevels::setHardLevels(bool hard)
{
  pf::set_levels_hard(hard);

  levels.init();
  updateLevels();
}


void MenuLevels::onAct(UIControl *ctrl)
{
  if (ctrl == &btnBack)
  {
    ba::move_to_scene(MenuMain::getMenu(), true);
  }
  else if (ctrl == &btnNormal)
  {
    setHardLevels(false);
  }
  else if (ctrl == &btnHard)
  {
    setHardLevels(true);
  }
  else if (ctrl == &levels)
  {
    int levelId = levels.getLevelId();

    if (levelId == -1)
    {
      pf::buy_levels();
      levels.init();
    }
    else
    {
      ba::start_level(levelId);
    }
  }
}


bool MenuLevels::onPointerDown(int idx, int x, int y)
{
  if (ba::is_cheats() && x < 100 && y < 40)
  {
    pf::cheat_open_levels(50);
    levels.init();
    updateLevels();
    return true;
  }

  return UIMenu::onPointerDown(idx, x, y);
}
