#ifndef __BONUS_BALL_H__
#define __BONUS_BALL_H__

#include "../bonuses.h"


class BonusSticky
  :public IBonus
{
public:
  BonusSticky(int id):IBonus(id) {}

  virtual void start();
  virtual void end();
};


class BonusGravity
  :public IBonus
{
public:
  BonusGravity(int id):IBonus(id) {}

  virtual void start();
  virtual void end();

  virtual int getTime() { return 10000; }
};


class BonusFireBall
  :public IBonus
{
public:
  BonusFireBall(int id):IBonus(id) {}

  virtual void start();
  virtual void end();

  virtual int getTime() { return 10000; }
};


class BonusPlasmaBall
  :public IBonus
{
public:
  BonusPlasmaBall(int id):IBonus(id) {}

  virtual void start();
  virtual void end();

  virtual int getTime() { return 10000; }
};


class BonusGhostBall
  :public IBonus
{
public:
  BonusGhostBall(int id):IBonus(id) {}

  virtual void start();
  virtual void end();

  virtual int getTime() { return 10000; }
};


#endif //__BONUS_BALL_H__
