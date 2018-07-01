#include "explosion.h"
#include "../res.h"

#include <epod.h>
#include <epod_graphics.h>


ExploManager::ExploManager()
  :m_partMarker(0)
{
  m_parts.resize(40);
}

void ExploManager::cast(const Point2f &pos)
{
  //m_parts[m_partMarker].init(pos, type);

  //m_partMarker++;
  //if (m_partMarker >= m_parts.size())
  //  m_partMarker = 0;

  for (int i=0; i<m_parts.size(); ++i)
  {
    Point2f dir(rand() % 100 / 50.f - 1.f, rand() % 100 / 50.f - 1.f);
    m_parts[i].init(pos, dir);
  }
}

void ExploManager::update(long dt)
{
  for (int i=0; i<m_parts.size(); ++i)
    m_parts[i].update(dt);
}

void ExploManager::render(ep::IGraphics *graphics)
{
  for (int i=0; i<m_parts.size(); ++i)
      m_parts[i].render(graphics);
}


///////////////////////////////////////////////////////////////////////////////


ExploPart::ExploPart()
  :m_color(0xFFFFFFFF)
{
}

void ExploPart::init(const Point2f &pos, const Point2f &dir)
{
  m_pos = pos;
  m_dir = dir;

  m_timer.start(1000);

  m_speed = 0.03f + 0.08f * (rand() % 100) / 100.f;
  //m_color = 0xFFFFD800;
  m_color = 0xFF000000 | (0x80 + rand() % 0x80) << 16 | (0x80 + rand() % 0x80) << 8 | (rand() % 0x60);
}

void ExploPart::update(long dt)
{
  if (m_timer.isEnd())
    return;

  m_timer.update(dt);
  m_pos += m_dir * dt * m_speed;
  m_speed -= 0.00005 * dt;

  int alpha = m_timer.getVal(230);
  m_color = (m_color & 0x00FFFFFF) | (alpha << 24);
}

void ExploPart::render(ep::IGraphics *graphics)
{
  if (m_timer.isEnd())
    return;

  graphics->drawTile(m_pos.x, m_pos.y, &::res.mTiles.star3, m_color, ep::A_CENTER);
}
