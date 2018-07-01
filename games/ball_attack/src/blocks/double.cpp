#include "double.h"
#include "../levelStats.h"
#include "../res.h"
#include "../fx/effects.h"

#include <epod.h>
#include <epod_graphics.h>


void BlockDouble::reset()
{
  BlockSimple::reset();
  m_hits = 0;
}


void BlockDouble::render(ep::IGraphics *graphics)
{
  if (m_dead)
    return;

  graphics->drawTile(m_bbox.left(), m_bbox.top(), getBaseTile(), m_color);
  graphics->drawTile(m_bbox.left(), m_bbox.top(), m_hits ? &::res.mTiles.picon_m1_b : &::res.mTiles.picon_m1_a, m_color);
}


void BlockDouble::collide(const Point2f &from, bool power)
{
  m_hits++;

  Point2f dir = m_bbox.center() - from;
  dir.normalize();

  if (power)
  {
    m_dead = true;
    ba::stats().addScores(10);

    fx::cast_fraction(getBaseTile(), m_bbox.center(), dir);
    fx::cast_fraction((m_hits == 1) ? &::res.mTiles.picon_m1_a : &::res.mTiles.picon_m1_b, m_bbox.center(), dir);
    return;
  }

  if (m_hits == 1)
  {
    fx::cast_fraction(&::res.mTiles.picon_m1_a, m_bbox.center(), dir);
  }
  else if (m_hits == 2)
  {
    m_dead = true;
    ba::stats().addScores(10);

    fx::cast_fraction(getBaseTile(), m_bbox.center(), dir);
    fx::cast_fraction(&::res.mTiles.picon_m1_b, m_bbox.center(), dir);
  }
}


///////////////////////////////////////////////////////////////////////////////


void BlockTriple::render(ep::IGraphics *graphics)
{
  if (m_dead)
    return;

  graphics->drawTile(m_bbox.left(), m_bbox.top(), getBaseTile(), m_color);

  const ep::Tile *tile = NULL;
  if (!m_hits)
    tile = &::res.mTiles.picon_m2_a1;
  else if (m_hits == 1)
    tile = &::res.mTiles.picon_m2_a2;
  else if (m_hits == 2)
    tile = &::res.mTiles.picon_m2_a3;

  graphics->drawTile(m_bbox.left(), m_bbox.top(), tile, m_color);
}


void BlockTriple::collide(const Point2f &from, bool power)
{
  m_hits++;

  Point2f dir = m_bbox.center() - from;
  dir.normalize();

  if (power)
  {
    m_dead = true;
    ba::stats().addScores(10);

    const ep::Tile *tile = NULL;
    if (!m_hits)
      tile = &::res.mTiles.picon_m2_a1;
    else if (m_hits == 1)
      tile = &::res.mTiles.picon_m2_a2;
    else if (m_hits == 2)
      tile = &::res.mTiles.picon_m2_a3;

    fx::cast_fraction(getBaseTile(), m_bbox.center(), dir);
    fx::cast_fraction(tile, m_bbox.center(), dir);
    return;
  }

  if (m_hits == 1)
  {
    fx::cast_fraction(&::res.mTiles.picon_m2_b1, m_bbox.center(), dir);
  }
  else if (m_hits == 2)
  {
    fx::cast_fraction(&::res.mTiles.picon_m2_b2, m_bbox.center(), dir);
  }
  else if (m_hits == 3)
  {
    m_dead = true;
    ba::stats().addScores(15);

    fx::cast_fraction(getBaseTile(), m_bbox.center(), dir);
    fx::cast_fraction(&::res.mTiles.picon_m2_a3, m_bbox.center(), dir);
  }
}
