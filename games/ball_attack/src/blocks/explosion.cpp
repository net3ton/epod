#include "explosion.h"
#include "../levels.h"
#include "../levelBase.h"
#include "../levelStats.h"
#include "../res.h"

#include <epod.h>
#include <epod_graphics.h>

enum
{
  DIST = 50,
};


BlockExplo::BlockExplo()
  :m_bombColor(0)
{
  m_iconTimer.startCycled(2000);
}


void BlockExplo::render(ep::IGraphics *graphics)
{
  if (m_dead)
    return;

  graphics->drawTile(m_bbox.left(), m_bbox.top(), getBaseTile(), m_color);
  graphics->drawTile(m_bbox.left(), m_bbox.top(), &::res.mTiles.picon_bomb_a, m_color);

  graphics->drawTile(m_bbox.left(), m_bbox.top(), &::res.mTiles.picon_bomb_b, m_bombColor);
}


void BlockExplo::collide(const Point2f &from, bool power)
{
  m_dead = true;
  ba::stats().addScores(20);

  ba::level()->exploBlocks(getPos(), DIST);
}
