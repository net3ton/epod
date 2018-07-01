#include "bonuses.h"
#include "bonusManager.h"
#include "levels.h"
#include "levelBase.h"
#include "levelStats.h"
#include "rocket.h"
#include "res.h"

#include "bonuses/simple.h"
#include "bonuses/speed.h"
#include "bonuses/rocket.h"
#include "bonuses/ball.h"

#include <epod.h>


IBonus *create_bonus(int id)
{
  if (id == BONUS_LIFE)
    return new BonusLife(id);
  if (id == BONUS_SPEEDUP)
    return new BonusSpeedUp(id);
  if (id == BONUS_SPEEDDOWN)
    return new BonusSpeedDown(id);
  if (id == BONUS_ROCKETBIG)
    return new BonusRocketBig(id);
  if (id == BONUS_ROCKETSMALL)
    return new BonusRocketSmall(id);
  if (id == BONUS_STICKY)
    return new BonusSticky(id);
  if (id == BONUS_FIREBALL)
    return new BonusFireBall(id);
  if (id == BONUS_PLASMABALL)
    return new BonusPlasmaBall(id);
  if (id == BONUS_SHIELD)
    return new BonusShield(id);
  if (id == BONUS_GHOSTBALL)
    return new BonusGhostBall(id);
  if (id == BONUS_GRAVITY)
    return new BonusGravity(id);

  ep::logError("Unknown bonus id (%d) creation!", id);
  return NULL;
}


void destroy_bonus(IBonus *bonus)
{
  delete bonus;
}


const ep::Tile *get_bonus_drop_tile(int id)
{
  if (id == BONUS_LIFE)
    return &::res.mTiles.bonus_life;
  if (id == BONUS_SPEEDUP)
    return &::res.mTiles.bonus_speedup;
  if (id == BONUS_SPEEDDOWN)
    return &::res.mTiles.bonus_speeddown;
  if (id == BONUS_ROCKETBIG)
    return &::res.mTiles.bonus_rocketbig;
  if (id == BONUS_ROCKETSMALL)
    return &::res.mTiles.bonus_rocketsmall;
  if (id == BONUS_STICKY)
    return &::res.mTiles.bonus_sticky;
  if (id == BONUS_FIREBALL)
    return &::res.mTiles.bonus_fireball;
  if (id == BONUS_PLASMABALL)
    return &::res.mTiles.bonus_plasmaball;
  if (id == BONUS_SHIELD)
    return &::res.mTiles.bonus_shield;
  if (id == BONUS_GHOSTBALL)
    return &::res.mTiles.bonus_ghostball;
  if (id == BONUS_GRAVITY)
    return &::res.mTiles.bonus_gravity;

  return NULL;
}

const char *get_bonus_descr(int id)
{
  if (id == BONUS_LIFE)
    return loc::BONUS_LIFE_DESC;
  if (id == BONUS_SPEEDUP)
    return loc::BONUS_SPEEDUP_DESC;
  if (id == BONUS_SPEEDDOWN)
    return loc::BONUS_SPEEDDOWN_DESC;
  if (id == BONUS_ROCKETBIG)
    return loc::BONUS_ROCKETBIG_DESC;
  if (id == BONUS_ROCKETSMALL)
    return loc::BONUS_ROCKETSMALL_DESC;
  if (id == BONUS_STICKY)
    return loc::BONUS_STICKY_DESC;
  if (id == BONUS_FIREBALL)
    return loc::BONUS_FIREBALL_DESC;
  if (id == BONUS_PLASMABALL)
    return loc::BONUS_PLASMABALL_DESC;
  if (id == BONUS_SHIELD)
    return loc::BONUS_SHIELD_DESC;
  if (id == BONUS_GHOSTBALL)
    return loc::BONUS_GHOSTBALL_DESC;
  if (id == BONUS_GRAVITY)
    return loc::BONUS_GRAVITY_DESC;

  return "";
}


///////////////////////////////////////////////////////////////////////////////


BonusManager::BonusManager()
  :DROP_SPEED(0.1f)
  ,WIDTH2(10.f)
  ,m_dropIndex(0)
{
  m_drops.resize(15);
}


void BonusManager::reset()
{
  for (int i=0; i<m_drops.size(); ++i)
    m_drops[i].dead = true;

  for (int i=0; i<m_bonuses.size(); ++i)
  {
    IBonus *bonus = m_bonuses[i];

    if (bonus->isEnd())
      continue;

    bonus->end();
    destroy_bonus(bonus);
  }

  m_bonuses.clear();
}


bool BonusManager::checkCollision(int x, int y)
{
  for (int i=0; i<m_drops.size(); ++i)
  {
    if (m_drops[i].dead)
      continue;

    if (fabs(m_drops[i].pos.x - x) <= WIDTH2 || fabs(m_drops[i].pos.y - y) <= WIDTH2)
      return true;
  }

  return false;
}


void BonusManager::drop(int id, int x, int y)
{
  if (x < WIDTH2)
    x = WIDTH2;
  else if (x > ep::get_width() - WIDTH2)
    x = ep::get_width() - WIDTH2;

  if (checkCollision(x, y))
    return;

  BonusDrop &drop = m_drops[m_dropIndex];

  drop.icon = get_bonus_drop_tile(id);

  if (!drop.icon)
    return;

  drop.id = id;
  drop.pos.set(x, y);
  drop.dead = false;

  m_dropIndex++;
  if (m_dropIndex >= m_drops.size())
    m_dropIndex = 0;
}


void BonusManager::dropRandom(int x, int y)
{
  int id = rand() % 20;
  drop(id, x, y);
}


bool BonusManager::endBonus(int id)
{
  IBonus *bonus = getBonus(id);
  if (bonus)
  {
    bonus->end();
    return true;
  }

  return false;
}


IBonus *BonusManager::getBonus(int id)
{
  for (int i=0; i<m_bonuses.size(); ++i)
  {
    IBonus *bonus = m_bonuses[i];

    if (bonus->getId() == id && !bonus->isEnd())
      return bonus;
  }

  return NULL;
}


void BonusManager::update(long dt)
{
  int ind = 0;
  while (ind < m_bonuses.size())
  {
    m_bonuses[ind]->update(dt);

    if (m_bonuses[ind]->isEnd())
    {
      destroy_bonus(m_bonuses[ind]);
      m_bonuses.erase(m_bonuses.begin() + ind);
      continue;
    }

    ind++;
  }

  ///

  for (int i=0; i<m_drops.size(); ++i)
  {
    if (m_drops[i].dead)
      continue;

    BonusDrop &drop = m_drops[i];

    drop.pos.y += dt * DROP_SPEED;

    if (ba::level()->getRocket().collideBonus(drop.pos))
    {
      drop.dead = true;

      ba::stats().onBonus();

      IBonus *bonus = getBonus(drop.id);
      if (bonus)
      {
        // restart bonus
        bonus->start();
        continue;
      }

      bonus = create_bonus(drop.id);
      if (!bonus)
      {
        ep::logError("NULL bonus (%d) creation!", drop.id);
        continue;
      }

      bonus->start();

      m_bonuses.push_back(bonus);
      continue;
    }

    if (drop.pos.y >= 480)
      drop.dead = true;
  }
}


void BonusManager::render(ep::IGraphics *graphics)
{
  //for (int i=0; i<m_bonuses.size(); ++i)
  //  m_bonuses[i]->render(graphics);

  for (int i=0; i<m_drops.size(); ++i)
  {
    if (m_drops[i].dead || !m_drops[i].icon)
      continue;

    graphics->drawTile(m_drops[i].pos.x, m_drops[i].pos.y, m_drops[i].icon, 0xFFFFFFFF, ep::A_CENTER);
  }
}
