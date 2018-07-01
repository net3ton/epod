#include "buttonMenu.h"
#include "../res.h"


ButtonMenu::ButtonMenu(UIMenu *menu, int x, int y, const char *text)
  :UIButton(menu)
{
  setSound(&::res.mSounds.button_click);
  setTiles(&::res.mTiles.btn_menu, &::res.mTiles.btn_menu_pressed, &::res.mTiles.btn_menu);
  setPos(x, y);
  setSize(190, 50);
  setFont(&::res.mFonts.main);
  setText(text);
}


///////////////////////////////////////////////////////////////////////////////


ButtonTile::ButtonTile(UIMenu *menu, int x, int y, ep::Tile *icon)
  :UIButton(menu)
  ,m_tileIcon(icon)
{
  setSound(&::res.mSounds.button_click);
  setTiles(&::res.mTiles.btn_small, &::res.mTiles.btn_small_pressed, &::res.mTiles.btn_small_disabled);
  setPos(x, y);
  setSize(50, 50);
}

void ButtonTile::render(ep::IGraphics *graph)
{
  UIButton::render(graph);
  graph->drawTile(m_center.x, m_center.y, m_tileIcon, m_color, ep::A_CENTER);
}


///////////////////////////////////////////////////////////////////////////////


ButtonTileTimer::ButtonTileTimer(UIMenu *menu, int x, int y, ep::Tile *icon)
  :ButtonTile(menu, x, y, icon)
{
}

void ButtonTileTimer::render(ep::IGraphics *graph)
{
  ButtonTile::render(graph);

  if (m_pressed)
  {
    int width = ::res.mTiles.btn_small_bar1.width;
    int xoff = (m_size.x - width) / 2;
    int posx = m_pos.x + xoff;
    int posy = m_pos.y - 25;

    graph->drawTile(posx, posy, &::res.mTiles.btn_small_bar2);

    Rect2i clip(0, 0, posx + m_timerDelay.getInvVal(width), ep::get_height());
    Rect2i old = graph->setClip(clip);
    graph->drawTile(posx, posy, &::res.mTiles.btn_small_bar1);
    graph->setClip(old);
  }
}

void ButtonTileTimer::update(long dt)
{
  if (m_timerDelay.update(dt))
  {
    m_pressed = false;
    onClick();
  }
}

void ButtonTileTimer::onPointerDown(int x, int y)
{
  m_pressed = true;
  m_timerDelay.start(1000);
}

void ButtonTileTimer::onPointerUp(int x, int y)
{
  m_pressed = false;
  m_timerDelay.end();
}

void ButtonTileTimer::onFocus(bool focus)
{
  if (!focus)
  {
    m_pressed = false;
    m_timerDelay.end();
  }
}


///////////////////////////////////////////////////////////////////////////////


ButtonHud::ButtonHud(UIMenu *menu, int x, int y, ep::Tile *icon)
  :ButtonTile(menu, x, y, icon)
{
  setTiles(&::res.mTiles.btn_hud, &::res.mTiles.btn_hud_pressed, &::res.mTiles.btn_hud);
  setSize(26, 26); 
}


///////////////////////////////////////////////////////////////////////////////


ButtonCheck::ButtonCheck(UIMenu *menu, int x, int y, ep::Tile *icon)
  :ButtonTile(menu, x, y, icon)
  ,m_checked(true)
  ,m_tileNormal(NULL)
  ,m_tileDisabled(NULL)
{
  m_tileNormal = m_tiles[BUTTON_NORMAL];
  m_tileDisabled = m_tiles[BUTTON_DISABLED];
}

void ButtonCheck::setChecked(bool checked)
{
  if (m_checked == checked)
    return;

  m_checked = checked;
  m_tiles[BUTTON_NORMAL] = checked ? m_tileNormal : m_tileDisabled;
}

void ButtonCheck::onClick()
{
  setChecked(!m_checked);
  UIButton::onClick();
}
