#include "button.h"
#include "../uiMenu.h"

#include <epod_graphics.h>
#include <epod_sound.h>


UIButton::UIButton(UIMenu *menu)
  :UIControl(menu)
  ,m_pressed(false)
{
  setSound(0);
  setTiles(0, 0, 0);

  m_textRenderer.setAlign(ep::A_CENTER);
}


void UIButton::setTiles(ep::Tile *normal, ep::Tile *pressed, ep::Tile *disabled)
{
  m_tiles[BUTTON_NORMAL] = normal;
  m_tiles[BUTTON_PRESSED] = pressed;
  m_tiles[BUTTON_DISABLED] = disabled;
}


void UIButton::setPos(int x, int y)
{
  UIControl::setPos(x, y);
  updatePos();
}


void UIButton::setSize(int w, int h)
{
  UIControl::setSize(w, h);
  updatePos();
}


void UIButton::updatePos()
{
  m_center.set(m_pos.x + m_size.x/2, m_pos.y + m_size.y/2);
  m_textRenderer.setPos(m_center.x, m_center.y);
}


void UIButton::render(ep::IGraphics *graph)
{
  //graph->drawFillRect(m_pos.x, m_pos.y, m_size.x, m_size.y, 0xFFAAAAAA);

  int state = m_pressed ? BUTTON_PRESSED : isEnabled() ? BUTTON_NORMAL : BUTTON_DISABLED;

  ep::Tile *tile = m_tiles[state];
  if (tile)
    graph->drawTile(m_center.x, m_center.y, tile, m_color, ep::A_CENTER);

  m_textRenderer.draw(graph);
}


void UIButton::onFocus(bool focus)
{
  if (!focus)
    m_pressed = false;
}


void UIButton::onPointerDown(int x, int y)
{
  m_pressed = true;
}


void UIButton::onPointerUp(int x, int y)
{
  if (!m_pressed)
    return;

  m_pressed = false;
  onClick();
}


void UIButton::onClick()
{
  if (m_clickSnd)
    m_clickSnd->play();

  if (m_menu)
    m_menu->onAct(this);
}
