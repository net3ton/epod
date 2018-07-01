#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "../uiControl.h"

#include <epod_font.h>
#include <epod.h>

namespace ep { class ISound; }
namespace ep { struct Tile; }


class UIButton
  :public UIControl
{
public:
  UIButton(UIMenu *menu);

  virtual void render(ep::IGraphics *graph);

  virtual void onPointerDown(int x, int y);
  virtual void onPointerUp(int x, int y);
  virtual void onFocus(bool focus);

  virtual void setPos(int x, int y);
  virtual void setSize(int w, int h);

protected:
  virtual void onClick();

  void setTiles(ep::Tile *normal, ep::Tile *pressed, ep::Tile *disabled);
  void setSound(ep::ISound *click) { m_clickSnd = click; }
  void setFont(ep::Font *font) { m_textRenderer.setFont(font); }
  void setText(const char *str) { m_textRenderer.setStr(str); }

  void updatePos();
  Point2i m_center;

  bool m_pressed;

  enum
  {
    BUTTON_NORMAL = 0,
    BUTTON_PRESSED,
    BUTTON_DISABLED,

    STATE_COUNT,
  };

  ep::ISound *m_clickSnd;
  ep::Tile *m_tiles[STATE_COUNT];
  ep::TextRenderer m_textRenderer;
};

#endif //__UI_BUTTON_H__
