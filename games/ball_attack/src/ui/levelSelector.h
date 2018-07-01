#ifndef __UI_LEVEL_SELECTOR_H__
#define __UI_LEVEL_SELECTOR_H__

#include "button.h"
#include <epod.h>


class LevelSelector
  :public UIButton
{
public:
  LevelSelector();

  enum
  {
    STATE_DISABLED,
    STATE_NEW,
    STATE_OPENED,
  };

  void init(UIMenu *menu);
  void setState(int id, int state, int stars);

  int getLevelId() const { return m_id; }

  virtual void render(ep::IGraphics *graph);

private:
  int m_id;
  int m_state;
  int m_stars;
};


class ButtonBuyLevels
  :public UIButton
{
public:
  ButtonBuyLevels(UIMenu *menu);
};


#endif //__UI_LEVEL_SELECTOR_H__
