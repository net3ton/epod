#ifndef __BONUS_ROCKET_H__
#define __BONUS_ROCKET_H__

#include "../bonuses.h"


class BonusRocketBig
  :public IBonus
{
public:
  BonusRocketBig(int id):IBonus(id) {}

  virtual void start();
  virtual void end();
};


class BonusRocketSmall
  :public IBonus
{
public:
  BonusRocketSmall(int id):IBonus(id) {}

  virtual void start();
  virtual void end();
};


#endif //__BONUS_ROCKET_H__
