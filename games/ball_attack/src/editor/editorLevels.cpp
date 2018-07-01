#include "editor.h"
#include "editorLevels.h"
#include "../main.h"
#include "../res.h"

MENU_SINGLETON_IMPLE(MenuEditorLevels)

MenuEditorLevels::MenuEditorLevels()
  :m_editor(NULL), m_toSave(true)
  ,txtLabel(this, 160, 24, "", ep::A_CENTER)
  ,btnBack(this, 5, 425, &::res.mTiles.icon_back)
  ,levels(this)
{
}


void MenuEditorLevels::setEditor(LevelEditor *editor, bool to_save)
{
  m_editor = editor;
  m_toSave = to_save;
  txtLabel.setText(to_save ? "Saving to..." : "Loading from...");
}


void MenuEditorLevels::onAct(UIControl *ctrl)
{
  if (ctrl == &levels)
  {
    int id = levels.getLevelId();

    if (m_toSave)
      m_editor->save(id);
    else
      m_editor->load(id);
  }

  m_editor->showLevelsMenu(false, true);
}
