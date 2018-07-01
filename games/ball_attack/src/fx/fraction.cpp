#include "fraction.h"

#include <epod.h>
#include <epod_graphics.h>


FractionManager::FractionManager()
  :partMarker(0)
{
  parts.resize(40);
}

void FractionManager::cast(const ep::Tile *tile, const Point2f &pos, const Point2f &dir)
{
  parts[partMarker].init(tile, pos, dir, 300);

  partMarker++;
  if (partMarker >= parts.size())
    partMarker = 0;
}

void FractionManager::update(long dt)
{
  for (int i=0; i<parts.size(); ++i)
    parts[i].update(dt);
}

void FractionManager::render(ep::IGraphics *graphics)
{
  for (int i=0; i<parts.size(); ++i)
      parts[i].render(graphics);
}


///////////////////////////////////////////////////////////////////////////////


Fraction::Fraction()
  :m_tile(0)
  ,m_color(0xFFFFFFFF)
{
}

void Fraction::init(const ep::Tile *tile, const Point2f &pos, const Point2f &dir, int time)
{
  m_tile = tile;
  m_pos = pos;
  m_dir = dir;
  m_color = 0xFFFFFFFF;
  m_timer.start(time);
}

void Fraction::update(long dt)
{
  if (m_timer.isEnd())
    return;

  m_timer.update(dt);
  m_pos += m_dir * dt * 0.05f;

  int alpha = m_timer.getVal(255);
  m_color =( m_color & 0x00FFFFFF) | (alpha << 24);
}

void Fraction::render(ep::IGraphics *graphics)
{
  if (m_timer.isEnd() || !m_tile)
    return;

  graphics->drawTile(m_pos.x, m_pos.y, m_tile, m_color, ep::A_CENTER);
}
