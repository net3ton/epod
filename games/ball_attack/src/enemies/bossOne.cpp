#include "bossOne.h"


EnemyBossOne::EnemyBossOne()
  :m_speedX(0.05f)
{
}


void EnemyBossOne::update(long dt)
{
  Point2f dir(m_speedX * dt, 0.f);
  move(dir);

  if (m_bbox.right() >= 320)
    m_speedX = -0.05f;
  else if (m_bbox.left() <= 0)
    m_speedX = 0.05f;
}
