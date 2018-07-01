#include "magnet.h"
#include "../levels.h"
#include "../levelBase.h"
#include "../levelStats.h"
#include "../res.h"

#include <epod.h>
#include <epod_graphics.h>

const float POWER_IN = 0.02f;
const float POWER_OUT = -0.04f;

const float RADIUS_IN = 100.f;
const float RADIUS_OUT = 70.f;


void BlockMagnetIn::init(int x, int y)
{
  BlockSimple::init(x, y);
  initMagnet(RADIUS_IN, POWER_IN);
}


void BlockMagnetIn::initMagnet(float radius, float power)
{
  m_bboxEffect.expand(m_bbox, radius);
  m_power = power;
}


void BlockMagnetIn::update(long dt)
{
  BlockSimple::update(dt);

  const Point2f &pos = ba::level()->getBall().getPos();

  if (m_bboxEffect.test(pos.x, pos.y))
  {
    Point2f dir = getPos() - pos;
    dir.normalize();
    ba::level()->getBall().modDir(dir * m_power);
  }
}


void BlockMagnetIn::render(ep::IGraphics *graphics)
{
  if (m_dead)
    return;

  graphics->drawTile(m_bbox.left(), m_bbox.top(), getBaseTile(), m_color);
  graphics->drawTile(m_bbox.left(), m_bbox.top(), &::res.mTiles.picon_magnet_b, m_color);
}


///////////////////////////////////////////////////////////////////////////////


void BlockMagnetOut::init(int x, int y)
{
  BlockSimple::init(x, y);
  initMagnet(RADIUS_OUT, POWER_OUT);
}


void BlockMagnetOut::render(ep::IGraphics *graphics)
{
  if (m_dead)
    return;

  graphics->drawTile(m_bbox.left(), m_bbox.top(), getBaseTile(), m_color);
  graphics->drawTile(m_bbox.left(), m_bbox.top(), &::res.mTiles.picon_magnet_a, m_color);
}
