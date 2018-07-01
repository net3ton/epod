#include "rocket.h"
#include "../levels.h"
#include "../levelBase.h"


void BonusRocketBig::start()
{
  if (ba::level()->endBonus(BONUS_ROCKETSMALL))
    return;

  ba::level()->getRocket().setSize(Rocket::SIZE_BIG);
  IBonus::start();
}

void BonusRocketBig::end()
{
  ba::level()->getRocket().setSize(Rocket::SIZE_NORMAL);
  IBonus::end();
}


void BonusRocketSmall::start()
{
  if (ba::level()->endBonus(BONUS_ROCKETBIG))
    return;

  ba::level()->getRocket().setSize(Rocket::SIZE_SMALL);
  IBonus::start();
}

void BonusRocketSmall::end()
{
  ba::level()->getRocket().setSize(Rocket::SIZE_NORMAL);
  IBonus::end();
}
