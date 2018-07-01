#include "star.h"
#include "../res.h"

#include <epod.h>
#include <epod_graphics.h>


StarManager::StarManager()
  :starMarker(0)
{
  stars.resize(40);
}

void StarManager::cast(const Point2f &pos)
{
  stars[starMarker].init(pos, 0xFFFFFFFF, 2000 + rand() % 4000);

  starMarker++;
  if (starMarker >= stars.size())
    starMarker = 0;
}

void StarManager::update(long dt)
{
  for (int i=0; i<stars.size(); ++i)
    stars[i].update(dt);
}

void StarManager::render(ep::IGraphics *graph)
{
  for (int i=0; i<stars.size(); ++i)
    stars[i].render(graph);
}


///////////////////////////////////////////////////////////////////////////////


Star::Star()
  :m_color(0xFFFFFFFF)
  ,m_dead(true)
{
}

void Star::init(const Point2f &pos, int color, int period)
{
  m_dead = false;
  m_pos = pos;
  m_color = color;
  m_timer.startCycled(period);
}

void Star::update(long dt)
{
  if (m_dead)
    return;

  m_timer.update(dt);
}

void Star::render(ep::IGraphics *graph)
{
  if (m_dead)
    return;

  int alpha = iabs(255 - m_timer.getVal(510));
  int color = (m_color & 0x00FFFFFF) | (alpha << 24);

  graph->drawTile(m_pos.x, m_pos.y, &::res.mTiles.star, color, ep::A_CENTER);
}
