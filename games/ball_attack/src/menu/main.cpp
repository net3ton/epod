#include "main.h"
#include "levels.h"
#include "about.h"
#include "../res.h"
#include "../main.h"
#include "../levels.h"
#include "../profile.h"
#include "../editor/editor.h"


MENU_SINGLETON_IMPLE(MenuMain)

MenuMain::MenuMain()
  :btnAction(this, 65, 140, loc::BTN_MODE_ACTION)
  ,btnSurvival(this, 65, 200, loc::BTN_MODE_SURVIVAL)
  ,btnLeaders(this, 5, 5, &::res.mTiles.icon_leaderboards)
  ,btnAchieves(this, 55, 5, &::res.mTiles.icon_achievs)
  ,btnOf(this, 105, 5, &::res.mTiles.icon_open_feint)
  ,btnEditor(this, 265, 5, &::res.mTiles.icon_editor)
  ,btnAbout(this, 265, 425, &::res.mTiles.icon_about)
  ,btnSound(this, 5, 425, &::res.mTiles.icon_sound)
  ,btnMusic(this, 55, 425, &::res.mTiles.icon_music)
{
}


void MenuMain::init()
{
  UIMenu::init();

  ep::snd::fade_to_music(&::res.mSounds.main_menu);
  
  btnSound.setChecked(pf::is_sound());
  btnMusic.setChecked(pf::is_music());
}


void MenuMain::update(long dt)
{
  UIMenu::update(dt);
}


void MenuMain::onAct(UIControl *ctrl)
{
  if (ctrl == &btnAction)
  {
    ba::move_to_scene(MenuLevels::getMenu(), false);
  }
  else if (ctrl == &btnSurvival)
  {
    ba::start_survival();
  }
  else if (ctrl == &btnAbout)
  {
    ba::move_to_scene(MenuAbout::getMenu(), false);
  }
  else if (ctrl == &btnMusic)
  {
    pf::set_music(btnMusic.isChecked());
  }
  else if (ctrl == &btnSound)
  {
    pf::set_sound(btnSound.isChecked());
  }
  else if (ctrl == &btnEditor)
  {
    ba::start_editor();
  }
}
