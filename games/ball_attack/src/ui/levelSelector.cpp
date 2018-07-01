#include "levelSelector.h"
#include "../uiMenu.h"
#include "../res.h"


LevelSelector::LevelSelector()
  :UIButton(0)
  ,m_id(0)
{
  setSound(&::res.mSounds.button_click);
  setTiles(&::res.mTiles.btn_level, &::res.mTiles.btn_level_pressed, &::res.mTiles.btn_level_disabled);
  setFont(&::res.mFonts.main);
  setSize(56, 56);
}

void LevelSelector::init(UIMenu *menu)
{
  m_menu = menu;
  if (m_menu)
    m_menu->addControl(this);
}

void LevelSelector::setState(int id, int state, int stars)
{
  setEnabled(state != STATE_DISABLED);
  m_id = id;
  m_state = state;
  m_stars = stars;

  if (state != STATE_DISABLED)
    m_textRenderer.printf("%d", m_id + 1);
  else
    m_textRenderer.clear();
}

void LevelSelector::render(ep::IGraphics *graph)
{
  UIButton::render(graph);

  if (m_state == STATE_DISABLED)
  {
    graph->drawTile(m_center.x, m_center.y, &::res.mTiles.btn_level_lock, m_color, ep::A_CENTER);
  }
  else if (m_state == STATE_OPENED)
  {
    for (int i=0; i<3; ++i)
    {
      int x = m_center.x - 10 + i * 10;
      int y = m_center.y + 22;

      ep::Tile *star = (i < m_stars) ? &::res.mTiles.btn_level_star_active : &::res.mTiles.btn_level_star_free;

      graph->drawTile(x + 2, y + 2, &::res.mTiles.btn_level_star_shadow, m_color, ep::A_CENTER);
      graph->drawTile(x, y, star, m_color, ep::A_CENTER);
    }
  }
}


///////////////////////////////////////////////////////////////////////////////


ButtonBuyLevels::ButtonBuyLevels(UIMenu *menu)
  :UIButton(menu)
{
  setSound(&::res.mSounds.button_click);
  setTiles(&::res.mTiles.btn_level_buy, &::res.mTiles.btn_level_buy_pressed, &::res.mTiles.btn_level_buy);
  setSize(56, 56);
}
