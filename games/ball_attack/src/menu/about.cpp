#include "about.h"
#include "main.h"
#include "../main.h"
#include "../profile.h"
#include "../res.h"


MENU_SINGLETON_IMPLE(MenuAbout)

MenuAbout::MenuAbout()
  :btnBack(this, 5, 425, &::res.mTiles.icon_back)
  ,txtName(this, 160, 90, "Epic Breaker", ep::A_CENTER)
{
}


void MenuAbout::init()
{
  UIMenu::init();
  ep::snd::fade_to_music(&::res.mSounds.credits);
}


void MenuAbout::onAct(UIControl *ctrl)
{
  if (ctrl == &btnBack)
  {
    ba::move_to_scene(MenuMain::getMenu(), true);
  }
}


bool MenuAbout::onPointerDown(int idx, int x, int y)
{
  if (ba::is_cheats() && x < 100 && y < 100)
  {
    pf::reset();
    onAct(&btnBack);
    return true;
  }

  return UIMenu::onPointerDown(idx, x, y);
}
