#ifndef __BONUS_SPEED_H__
#define __BONUS_SPEED_H__

#include "../bonuses.h"


class BonusSpeedUp
  :public IBonus
{
public:
  BonusSpeedUp(int id):IBonus(id) {}

  virtual void start();
  virtual void end();
};


class BonusSpeedDown
  :public IBonus
{
public:
  BonusSpeedDown(int id):IBonus(id) {}

  virtual void start();
  virtual void end();
};


#endif //__BONUS_SPEED_H__
