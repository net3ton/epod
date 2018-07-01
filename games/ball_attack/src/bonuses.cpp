#include "bonuses.h"
#include "levels.h"
#include "levelBase.h"


void IBonus::start()
{
  m_timer.start(getTime());
  ba::level()->addBonusToPanel(this);
}

void IBonus::end()
{
  m_timer.end();
  ba::level()->deleteBonusFromPanel(this);
}

void IBonus::update(long dt)
{
  if (m_timer.update(dt))
    end();
}
