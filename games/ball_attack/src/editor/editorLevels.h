#ifndef __LEVEL_EDITOR_SELECTOR_H__
#define __LEVEL_EDITOR_SELECTOR_H__

#include "../uiMenu.h"
#include "../ui/buttonMenu.h"
#include "../ui/levelSlider.h"
#include "../ui/textMenu.h"

#include <epod_font.h>

class LevelEditor;


class MenuEditorLevels:
  public UIMenu
{
MENU_SINGLETON_DEF(MenuEditorLevels)

public:
  MenuEditorLevels();

  virtual void onAct(UIControl *ctrl);

  void setEditor(LevelEditor *editor, bool to_save);

private:
  LevelEditor *m_editor;
  bool m_toSave;

  TextMenu txtLabel;
  ButtonTile btnBack;
  LevelsSlider levels;
};


#endif //__LEVEL_EDITOR_SELECTOR_H__
