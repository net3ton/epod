#include "path.h"
#include "../res.h"
#include "../ball.h"

#include <epod.h>
#include <epod_graphics.h>


PathManager::PathManager()
  :m_starMarker(0)
{
  m_stars.resize(40);
}

void PathManager::cast(const Point2f &pos, int type)
{
  m_stars[m_starMarker].init(pos, type);

  m_starMarker++;
  if (m_starMarker >= m_stars.size())
    m_starMarker = 0;
}

void PathManager::update(long dt)
{
  for (int i=0; i<m_stars.size(); ++i)
    m_stars[i].update(dt);
}

void PathManager::render(ep::IGraphics *graphics)
{
  for (int i=0; i<m_stars.size(); ++i)
      m_stars[i].render(graphics);
}


///////////////////////////////////////////////////////////////////////////////


StarPath::StarPath()
  :m_tile(0)
  ,m_color(0xFFFFFFFF)
{
}

void StarPath::init(const Point2f &pos, int type)
{
  m_type = type;
  m_pos = pos;
  m_timer.start(600);

  if (m_type == Ball::TYPE_NORMAL)
  {
    m_tile = &::res.mTiles.star2;
    m_color = 0xFFFFFF80;
  }
  else if (m_type == Ball::TYPE_FIRE)
  {
    m_tile = &::res.mTiles.star3;
    m_color = 0xFFFFD800;
  }
  else if (m_type == Ball::TYPE_PLASMA)
  {
    m_tile = &::res.mTiles.star2;
    m_color = 0xFF7070FF;
  }
  else if (m_type == Ball::TYPE_GHOST)
  {
    m_tile = &::res.mTiles.star;
    m_color = 0xFFFFFFFF;
  }

  m_dir.set(rand() % 100 / 50.f - 1.f, rand() % 100 / 50.f - 1.f);
}

void StarPath::update(long dt)
{
  if (m_timer.isEnd())
    return;

  m_timer.update(dt);
  m_pos += m_dir * dt * 0.01f;

  int maxAlpha = 128;
  if (m_type == Ball::TYPE_FIRE || m_type == Ball::TYPE_PLASMA)
    maxAlpha = 230;

  int alpha = m_timer.getVal(maxAlpha);
  m_color = (m_color & 0x00FFFFFF) | (alpha << 24);
}

void StarPath::render(ep::IGraphics *graphics)
{
  if (m_timer.isEnd() || !m_tile)
    return;

  graphics->drawTile(m_pos.x, m_pos.y, m_tile, m_color, ep::A_CENTER);
}
