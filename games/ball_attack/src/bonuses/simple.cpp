#include "simple.h"
#include "../levels.h"
#include "../levelBase.h"
#include "../levelStats.h"


void BonusLife::start()
{
  ba::stats().addLife();
}


void BonusShield::start()
{
  ba::level()->activateShield(true);
  IBonus::start();
}

void BonusShield::end()
{
  ba::level()->activateShield(false);
  IBonus::end();
}
