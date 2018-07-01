#include "levelBase.h"
#include "levelStats.h"
#include "collision.h"
#include "blocks.h"
#include "res.h"


void LevelBase::boundsCollisionCheck()
{
  const Point2f &pos = m_ball.getPos();
  float radius = m_ball.getRadius();

  bool collided = false;
  if ((pos.x - radius) < m_bound1.x)
  {
    m_ball.setPosX(m_bound1.x + radius);
    m_ball.reflectDirRight();
    collided = true;
  }
  if ((pos.y - radius) < m_bound1.y)
  {
    m_ball.setPosY(m_bound1.y + radius);
    m_ball.reflectDirBottom();
    collided = true;
  }
  if ((pos.x + radius) > m_bound2.x)
  {
    m_ball.setPosX(m_bound2.x - radius);
    m_ball.reflectDirLeft();
    collided = true;
  }

  float bottomLevel = m_bound2.y;

  if (m_shield)
  {
    const Point2f &posLast = m_ball.getPosLast();

    if (pos.y < m_shieldLevel || posLast.y < m_shieldLevel)
      bottomLevel = m_shieldLevel;
  }

  if ((pos.y + radius) > bottomLevel)
  {
    m_ball.setPosY(bottomLevel - radius);
    m_ball.reflectDirTop();
    collided = true;

    if (!m_shield)
    {
      bool lifeUsed = ba::stats().useLife();
      m_ball.die();
      m_bonuses.reset();

      m_stateTimer.start(500);
      m_state = lifeUsed ? STATE_NEW_BALL : STATE_FAILED;
    }
  }

  if (collided)
    ::res.mSounds.ball_knock.play();
}


void LevelBase::blocksCollisionCheck(long dt)
{
  const Point2f &p1 = m_ball.getPosLast();
  const Point2f &p2 = m_ball.getPos();
  Point2f dir = p2 - p1;

  Aabb big;
  big.fromCircles(p1, p2, m_ball.getRadius());

  m_frameColliders.clear();
  m_frameSweepRes.clear();
  
  /// broad phase
  for (int i=0; i<m_blocks.size(); ++i)
  {
    IBlockBase *block = m_blocks[i];
    if (block->isDead())
      continue;

    if (block->testCollide(big))
      m_frameColliders.push_back(block);
  }

  if (m_frameColliders.empty())
    return;

  /// narrow phase
  for (int i=0; i<m_frameColliders.size(); ++i)
  {
    IBlockBase *block = m_frameColliders[i];

    float t = 0.f;
    Point2f n;
    if (block->sweepCollide(p1, dir, m_ball.getRadius(), t, n))
    {
      SweepRes res;
      res.block = block;
      res.t = t;
      res.n = n;

      m_frameSweepRes.push_back(res);
    }
  }

  if (m_frameSweepRes.empty())
    return;

  ///
  const float TIME_DELTA = 0.1f;
  float time = 1.f;
  IBlockBase *resBlock = NULL;
  Point2f norm;

  bool plasmaBall = m_ball.isPlasma();

  for (SweepResIter i=m_frameSweepRes.begin(), e=m_frameSweepRes.end(); i!=e; ++i)
  {
    SweepRes &res = (*i);

    if (res.t < time)
    {
      if (plasmaBall && res.block->isBreakable())
        continue;

      time = res.t;
      resBlock = res.block;
      norm = res.n;
    }
  }

  /// plasma ball
  if (plasmaBall)
  {
    float testTime = time - TIME_DELTA;

    for (SweepResIter i=m_frameSweepRes.begin(), e=m_frameSweepRes.end(); i!=e; )
    {
      SweepRes &res = (*i);

      if (res.t <= testTime && res.block->isBreakable())
      {
        res.block->collide(p2, true);
        m_frameSweepRes.erase(i++);
        continue;
      }

      ++i;
    }
  }

  if (!resBlock)
    return;

  /// move not to collide
  time -= TIME_DELTA;
  Point2f newPos = p1 + dir * time;
  m_ball.setPos(newPos);

  /// change direction
  Point2f normRes;

  for (SweepResIter i=m_frameSweepRes.begin(), e=m_frameSweepRes.end(); i!=e; ++i)
  {
    SweepRes &res = (*i);

    if (fabs(res.t - time) <= TIME_DELTA)
    {
      res.block->playKnock();
      normRes += res.n;

      /// ghost ball
      if (m_ball.isGhost())
        continue;

      res.block->collide(newPos, false);

      if (res.block->isDead())
        onBlockBreak(res.block->getPos().x, res.block->getPos().y);
    }
  }

  Point2f curDir = m_ball.getDir();
  Point2f newDir = reflect(curDir, normRes);
  m_ball.setDir(newDir);

  /// fire ball
  if (m_ball.isFire())
    exploBlocks(newPos, 35.f);
}


void LevelBase::exploBlocks(const Point2f &from, float dist)
{
  m_exposions.cast(from);

  Aabb exploBox(from, dist);

  for (int i=0; i<m_blocks.size(); ++i)
  {
    if (m_blocks[i]->isDead())
      continue;

    if (!m_blocks[i]->testCollide(exploBox))
      continue;

    Point2f distDir = m_blocks[i]->getPos() - from;
    float distSq = distDir.lengthSq();
    if (distSq <= dist * dist)
    {
      ExploBlock info;
      info.block = m_blocks[i];
      info.time = distSq / 10;
      info.from = from;

      m_exploBlocks.push_back(info);
    }
  }
}


void LevelBase::updateExplo(long dt)
{
  for (ExploIter i=m_exploBlocks.begin(), e=m_exploBlocks.end(); i!=e; )
  {
    ExploBlock &info = (*i);
    info.time -= dt;

    if (info.time <= 0)
    {
      IBlockBase *block = info.block;

      if (hasBlock(block) && !block->isDead())
        block->collide(info.from, false);

      m_exploBlocks.erase(i++);
      continue;
    }

    ++i;
  }
}
