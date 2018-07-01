#ifndef __BONUSES_H__
#define __BONUSES_H__

#include <timer_int.h>


enum
{
  BONUS_LIFE = 0,
  BONUS_SPEEDUP,
  BONUS_SPEEDDOWN,
  BONUS_ROCKETBIG,
  BONUS_ROCKETSMALL,
  BONUS_STICKY,
  BONUS_FIREBALL,
  BONUS_PLASMABALL,
  BONUS_SHIELD,
  BONUS_GHOSTBALL,
  BONUS_GRAVITY,

  BONUSES_COUNT,
};


class IBonus
{
public:
  IBonus(int id):m_id(id) {}
  virtual ~IBonus() {}

  virtual void start();
  virtual void end();

  virtual void update(long dt);

  float getProgress() const { return m_timer.getProgress(); }
  bool isEnd() const { return m_timer.isEnd(); }
  int getId() const { return m_id; }

protected:
  virtual int getTime() { return 15000; }

  int m_id;
  TimerInt m_timer;
};


#endif //__BONUSES_H__
 