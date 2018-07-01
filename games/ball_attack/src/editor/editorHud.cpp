#include "editor.h"
#include "editorHud.h"
#include "../res.h"
#include "../main.h"
#include "../menu/main.h"

MENU_SINGLETON_IMPLE(MenuEditorHud)


MenuEditorHud::MenuEditorHud()
  :btnClose(this, 290, 4, &::res.mTiles.icon_close)
  ,btnSave(this, 250, 4, &::res.mTiles.icon_save)
  ,btnLoad(this, 210, 4, &::res.mTiles.icon_load)
{
}


void MenuEditorHud::onAct(UIControl *ctrl)
{
  if (ctrl == &btnClose)
  {
    ba::move_to_scene(MenuMain::getMenu(), true);
  }
  else if (ctrl == &btnSave)
  {
    m_editor->showLevelsMenu(true, true);
  }
  else if (ctrl == &btnLoad)
  {
    m_editor->showLevelsMenu(true, false);
  }
}
