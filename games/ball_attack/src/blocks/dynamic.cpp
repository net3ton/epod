#include "dynamic.h"
//#include "../levels.h"
//#include "../levelBase.h"
//#include "../levelStats.h"
#include "../res.h"

#include <epod.h>
#include <epod_graphics.h>

const Point2f CENTER = Point2f(160.f, 166.f);
const float RAD_SPEED = PI * 0.00015f;


BlockDynamic::BlockDynamic()
{
}


void BlockDynamic::init(int x, int y)
{
  BlockSimple::init(x, y);
  m_initPos.set(x, y);
}


void BlockDynamic::reset()
{
  BlockSimple::reset();
  init(m_initPos.x, m_initPos.y);
}


void BlockDynamic::update(long dt)
{
  BlockSimple::update(dt);

  Point2f dir = getPos() - CENTER;

  const float alpha = RAD_SPEED * dt;
  const float rcos = cos(alpha);
  const float rsin = sin(alpha);

  Point2f newDir(dir.x * rcos - dir.y * rsin, dir.x * rsin + dir.y * rcos);
  newDir += CENTER;

  move(newDir - getPos());

  /*
  Point2f dir(m_speedX * dt, 0.f);
  move(dir);

  if (m_bbox.right() >= 320)
    m_speedX = -0.05f;
  else if (m_bbox.left() <= 0)
    m_speedX = 0.05f;
  */
}
