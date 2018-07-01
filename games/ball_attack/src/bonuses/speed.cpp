#include "speed.h"
#include "../levels.h"
#include "../levelBase.h"


void BonusSpeedUp::start()
{
  if (ba::level()->endBonus(BONUS_SPEEDDOWN))
    return;

  ba::level()->getBall().setSpeed(350.f);
  IBonus::start();
}

void BonusSpeedUp::end()
{
  ba::level()->getBall().setSpeed(250.f);
  IBonus::end();
}


void BonusSpeedDown::start()
{
  if (ba::level()->endBonus(BONUS_SPEEDUP))
    return;

  ba::level()->getBall().setSpeed(150.f);
  IBonus::start();
}

void BonusSpeedDown::end()
{
  ba::level()->getBall().setSpeed(250.f);
  IBonus::end();
}
