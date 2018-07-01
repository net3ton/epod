#ifndef __BONUS_SIMPLE_H__
#define __BONUS_SIMPLE_H__

#include "../bonuses.h"


class BonusLife
  :public IBonus
{
public:
  BonusLife(int id):IBonus(id) {}

  virtual void start();
};


class BonusShield
  :public IBonus
{
public:
  BonusShield(int id):IBonus(id) {}

  virtual void start();
  virtual void end();
};


#endif //__BONUS_SIMPLE_H__
