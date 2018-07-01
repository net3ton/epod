#include "bonusPanel.h"
#include "bonusManager.h"
#include "bonuses.h"
#include "res.h"

#include <epod.h>


BonusPanel::BonusPanel()
{
}


int BonusPanel::findBonus(IBonus *bonus)
{
  for (int i=0; i<m_panel.size(); ++i)
    if (m_panel[i].bonus == bonus)
      return i;

  return -1;
}


void BonusPanel::addBonus(IBonus *bonus)
{
  int ind = findBonus(bonus);
  if (ind < 0)
  {
    BonusIcon info;
    info.bonus = bonus;
    info.left = 100 + m_panel.size() * 28;

    m_panel.push_back(info);
  }
}


void BonusPanel::deleteBonus(IBonus *bonus)
{
  int ind = findBonus(bonus);
  if (ind >= 0)
    m_panel.erase(m_panel.begin() + ind);
}


void BonusPanel::update(long dt)
{
  for (int i=0; i<m_panel.size(); ++i)
  {
    const float left = 100 + i * 28;

    if (m_panel[i].left > left)
    {
      m_panel[i].left -= 0.2f * dt;
      if (m_panel[i].left < left)
        m_panel[i].left = left;
    }
  }
}


void BonusPanel::render(ep::IGraphics *graphics)
{
  for (int i=0; i<m_panel.size(); ++i)
  {
    const ep::Tile *tile = get_bonus_drop_tile(m_panel[i].bonus->getId());
    if (tile)
    {
      const float left = m_panel[i].left;
      const float top = 17;
      const float size = 26;
      const float progress = size * m_panel[i].bonus->getProgress();

      graphics->drawTile(left, top, &::res.mTiles.bonus_back, 0xFFFFFFFF, ep::A_CENTER);
      graphics->drawTile(left, top, tile, 0xFFFFFFFF, ep::A_CENTER);
      graphics->drawFillRect(left - 13, top + 13 - progress, size, progress, 0xB4230E50);
    }
  }
}
