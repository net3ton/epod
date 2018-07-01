#ifndef __UI_BUTTON_MENU_H__
#define __UI_BUTTON_MENU_H__

#include "button.h"

#include <epod.h>
#include <timer_int.h>


class ButtonMenu
  :public UIButton
{
public:
  ButtonMenu(UIMenu *menu, int x, int y, const char *text);
};


class ButtonTile
  :public UIButton
{
public:
  ButtonTile(UIMenu *menu, int x, int y, ep::Tile *icon);

protected:
  virtual void render(ep::IGraphics *graph);

  ep::Tile *m_tileIcon;
};


class ButtonTileTimer
  :public ButtonTile
{
public:
  ButtonTileTimer(UIMenu *menu, int x, int y, ep::Tile *icon);

private:
  virtual void render(ep::IGraphics *graph);
  virtual void update(long dt);

  virtual void onPointerDown(int x, int y);
  virtual void onPointerUp(int x, int y);
  virtual void onFocus(bool focus);

  TimerInt m_timerDelay;
};


class ButtonHud
  :public ButtonTile
{
public:
  ButtonHud(UIMenu *menu, int x, int y, ep::Tile *icon);
};


class ButtonCheck
  :public ButtonTile
{
public:
  ButtonCheck(UIMenu *menu, int x, int y, ep::Tile *icon);

  void setChecked(bool checked);
  bool isChecked() const { return m_checked; }

private:
  virtual void onClick();

  bool m_checked;
  ep::Tile *m_tileNormal, *m_tileDisabled;
};

#endif //__UI_BUTTON_MENU_H__
