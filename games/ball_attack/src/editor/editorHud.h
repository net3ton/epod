#ifndef __LEVEL_EDITOR_HUD_H__
#define __LEVEL_EDITOR_HUD_H__

#include "../uiMenu.h"
#include "../ui/buttonMenu.h"
#include "../ui/textMenu.h"

class LevelEditor;


class MenuEditorHud:
  public UIMenu
{
MENU_SINGLETON_DEF(MenuEditorHud)

public:
  MenuEditorHud();

  virtual void onAct(UIControl *ctrl);

  void setEditor(LevelEditor *editor) { m_editor = editor; }

private:
  LevelEditor *m_editor;

  ButtonHud btnClose;
  ButtonHud btnSave, btnLoad;
};


#endif //__LEVEL_EDITOR_HUD_H__
