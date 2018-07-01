#include "simple.h"
#include "../levelStats.h"
#include "../fx/effects.h"
#include "../levels.h"
#include "../levelBase.h"
#include "../bonuses.h"
#include "../res.h"

#include <epod.h>
#include <epod_graphics.h>


BlockSimple::BlockSimple()
  :m_dead(false)
  ,m_color(0xFFFFFFFF)
{
}


void BlockSimple::showAnim()
{
  m_color = 0x00FFFFFF;
  m_timerDelay.start(rand() % 1000);
}


void BlockSimple::init(int x, int y)
{
  m_bbox.set(x, y, 17, 17);
}


void BlockSimple::reset()
{
  if (m_dead)
    showAnim();

  m_dead = false;
}


void BlockSimple::update(long dt)
{
  if (m_timerAnim.isWorking() || m_timerDelay.isWorking())
  {
    if (m_timerAnim.update(dt) || m_timerAnim.isWorking())
    {
      int alpha = 255 - m_timerAnim.getVal(255);
      m_color = (m_color & 0x00FFFFFF) | (alpha << 24); 
    }

    if (m_timerDelay.update(dt))
      m_timerAnim.start(500);
  }
}


void BlockSimple::render(ep::IGraphics *graphics)
{
  if (m_dead)
    return;

  graphics->drawTile(m_bbox.left(), m_bbox.top(), getBaseTile(), m_color);

  /*
  graphics->drawLine(m_bbox.left(), m_bbox.top(), m_bbox.right(), m_bbox.top(), 0xFFFFFFFF);
  graphics->drawLine(m_bbox.left(), m_bbox.bottom(), m_bbox.right(), m_bbox.bottom(), 0xFFFFFFFF);
  graphics->drawLine(m_bbox.left(), m_bbox.bottom(), m_bbox.left(), m_bbox.top(), 0xFFFFFFFF);
  graphics->drawLine(m_bbox.right(), m_bbox.bottom(), m_bbox.right(), m_bbox.top(), 0xFFFFFFFF);
  */
}

void BlockSimple::collide(const Point2f &from, bool power)
{
  m_dead = true;
  ba::stats().addScores(5);

  Point2f dir = m_bbox.center() - from;
  dir.normalize();

  //fx::cast_spark(m_bbox.center(), m_deadDir);
  fx::cast_fraction(getBaseTile(), m_bbox.center(), dir);
}

bool BlockSimple::testCollide(const Aabb &b)
{
  return m_bbox.test(b);
}

bool BlockSimple::sweepCollide(const Point2f &p, const Point2f &dir, float r, float &tmin, Point2f &n)
{
  return ::sweep_circle_aabb(p, dir, r, m_bbox, tmin, n);
}

void BlockSimple::playKnock()
{
  ::res.mSounds.ball_knock.play();
}


///////////////////////////////////////////////////////////////////////////////


void BlockStatic::render(ep::IGraphics *graphics)
{
  if (m_dead)
    return;

  graphics->drawTile(m_bbox.left(), m_bbox.top(), getBaseTile(), m_color);
  graphics->drawTile(m_bbox.left(), m_bbox.top(), &::res.mTiles.picon_block, m_color);
}

void BlockStatic::collide(const Point2f &from, bool power)
{
}


///////////////////////////////////////////////////////////////////////////////


void BlockBonus::render(ep::IGraphics *graphics)
{
  if (m_dead)
    return;

  graphics->drawTile(m_bbox.left(), m_bbox.top(), getBaseTile(), m_color);
  graphics->drawTile(m_bbox.left(), m_bbox.top(), &::res.mTiles.picon_bonus, m_color);
}

void BlockBonus::collide(const Point2f &from, bool power)
{
  BlockSimple::collide(from, power);

  const int POSITIVE_BONUSES_COUNT = 6;
  int bonuses[POSITIVE_BONUSES_COUNT] =
  {
    BONUS_SPEEDDOWN,
    BONUS_ROCKETBIG,
    BONUS_STICKY,
    BONUS_FIREBALL,
    BONUS_PLASMABALL,
    BONUS_SHIELD,
  };

  int id = bonuses[rand() % POSITIVE_BONUSES_COUNT];

  Point2f pos = getPos();
  ba::level()->dropBonus(id, pos.x, pos.y);
}
