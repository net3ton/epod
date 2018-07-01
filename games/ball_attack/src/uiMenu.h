#ifndef __UI_MENU_H__
#define __UI_MENU_H__

#include "sceneBase.h"
#include <vector>

#define MENU_SINGLETON_DEF(MenuClass) \
  public: \
    static MenuClass *getMenu(); \
  private: \
    static MenuClass *menu;

#define MENU_SINGLETON_IMPLE(MenuClass) \
  MenuClass *MenuClass::menu = NULL; \
  MenuClass *MenuClass::getMenu() \
  { \
    if (!menu) \
      menu = new MenuClass(); \
    return menu; \
  }

class UIControl;


class UIMenu
  :public BaseScene
{
public:
  UIMenu();

  virtual void init();
  virtual void destroy();

  virtual void update(long dt);
  virtual void render(ep::IGraphics *graph);

  virtual bool onPointerDown(int idx, int x, int y);
  virtual bool onPointerUp(int idx, int x, int y);
  virtual bool onPointerMove(int idx, int x, int y);

  virtual bool onKeyDown(int key) { return false; }
  virtual bool onKeyUp(int key) { return false; }

  virtual void onPause() {}
  virtual void onResume() {}

  /// trans anim events
  virtual void onMoveinEnd();
  virtual void onMoveoutStart();

  /// callbacks from controls
  virtual void onAct(UIControl *ctrl) {}

  ///
  void addControl(UIControl *ctrl);
  void captureFocus(bool capture) { m_focusCaptured = capture; }

  void lockControls(bool lock) { m_lockControls = lock; }

protected:
  std::vector<UIControl*> m_controls;
  UIControl *m_focused;
  bool m_focusCaptured;
  bool m_lockControls;
};

#endif //__UI_MENU_H__
