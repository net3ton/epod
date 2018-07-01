#ifndef __UI_LEVEL_SLIDER_H__
#define __UI_LEVEL_SLIDER_H__

#include "levelSelector.h"
#include "textMenu.h"
#include "../uiMenu.h"

#include <epod.h>
#include <timer_int.h>

class LevelsSlider;


class LavelsPageBase
  :public UIMenu
{
public:
  virtual void initPage(UIMenu *parent, LevelsSlider *slider, int first_id) = 0;
  virtual void setPos(int left) = 0;
  virtual void addStars(int &max, int &cur) = 0;
};


class LevelsPage
  :public LavelsPageBase
{
public:
  LevelsPage();

  virtual void initPage(UIMenu *parent, LevelsSlider *slider, int first_id);
  virtual void setPos(int left);
  virtual void addStars(int &max, int &cur);

  enum
  {
    LEVELS_COUNT = 25,
  };

protected:
  virtual void onAct(UIControl *ctrl);

  void getBtnPos(int idx, int &x, int &y);

  LevelSelector m_btns[LEVELS_COUNT];

  UIMenu *m_parent;
  LevelsSlider *m_slider;

  int m_starsCur;
};


class LevelsPageFirst
  :public LevelsPage
{
public:
  LevelsPageFirst();

  virtual void initPage(UIMenu *parent, LevelsSlider *slider, int first_id);
  virtual void render(ep::IGraphics *graph);
  virtual void setPos(int left);

private:
  virtual void onAct(UIControl *ctrl);

  ButtonBuyLevels m_btnBuy;

  int m_left;
};


class LevelsPageLast
  :public LavelsPageBase
{
public:
  LevelsPageLast();

  virtual void initPage(UIMenu *parent, LevelsSlider *slider, int first_id) {}
  virtual void setPos(int left);
  virtual void addStars(int &max, int &cur) {}

private:
  TextMenu txtMoreLevels;
};


///////////////////////////////////////////////////////////////////////////////


class LevelsSlider
  :public UIControl
{
public:
  LevelsSlider(UIMenu *menu);
  ~LevelsSlider();

  virtual void init();
  virtual void destroy();

  virtual void update(long dt);
  virtual void render(ep::IGraphics *graph);

  virtual void onPointerDown(int x, int y);
  virtual void onPointerUp(int x, int y);
  virtual void onPointerMove(int x, int y);

  virtual void onFocus(bool focus);

  void setLevelId(int id) { m_curLevelId = id; }
  int getLevelId() const { return m_curLevelId; }

  void getStars(int &max, int &cur) { max = m_starsMax; cur = m_starsCur; }

private:
  void updatePos(int delta);
  bool isVisiblePage(int idx);

  int m_curLevelId;
  int m_current;
  int m_next;

  bool m_pressed, m_moving;
  int m_startPos;

  int m_curDelta, m_moveDelta;
  TimerInt m_timer;

  int m_starsMax, m_starsCur;

  const int WIDTH;

  enum { COUNT = 3 };
  LavelsPageBase *m_pages[COUNT];
};

#endif //__UI_LEVEL_SLIDER_H__
