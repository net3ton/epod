#include "ball.h"
#include "../levels.h"
#include "../levelBase.h"


void BonusSticky::start()
{
  ba::level()->getBall().setSticky(true);
  IBonus::start();
}

void BonusSticky::end()
{
  ba::level()->getBall().setSticky(false);
  IBonus::end();
}


void BonusGravity::start()
{
  ba::level()->getBall().setGravity(true);
  IBonus::start();
}

void BonusGravity::end()
{
  ba::level()->getBall().setGravity(false);
  IBonus::end();
}


void BonusFireBall::start()
{
  ba::level()->endBonus(BONUS_PLASMABALL);
  ba::level()->endBonus(BONUS_GHOSTBALL);
  ba::level()->getBall().setType(Ball::TYPE_FIRE);
  IBonus::start();
}

void BonusFireBall::end()
{
  ba::level()->getBall().setType(Ball::TYPE_NORMAL);
  IBonus::end();
}


void BonusPlasmaBall::start()
{
  ba::level()->endBonus(BONUS_FIREBALL);
  ba::level()->endBonus(BONUS_GHOSTBALL);
  ba::level()->getBall().setType(Ball::TYPE_PLASMA);
  IBonus::start();
}

void BonusPlasmaBall::end()
{
  ba::level()->getBall().setType(Ball::TYPE_NORMAL);
  IBonus::end();
}


void BonusGhostBall::start()
{
  ba::level()->endBonus(BONUS_FIREBALL);
  ba::level()->endBonus(BONUS_PLASMABALL);
  ba::level()->getBall().setType(Ball::TYPE_GHOST);
  IBonus::start();
}

void BonusGhostBall::end()
{
  ba::level()->getBall().setType(Ball::TYPE_NORMAL);
  IBonus::end();
}
